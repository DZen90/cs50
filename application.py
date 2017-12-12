from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from passlib.context import CryptContext

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# configure context objec to use hashing algorithms
myctx = CryptContext(schemes=["sha256_crypt", "md5_crypt", "des_crypt"])
# myctx = CryptContext(schemes=["md5_crypt"])

@app.route("/")
@login_required
def index():
    
    # Get the stocks owned by the user
    rows = db.execute("SELECT name, SUM(amount) AS amount FROM operations WHERE user_id = :id GROUP BY name", 
                      id=session["user_id"])
    # Get current prices for stocks owned
    total_value = 0
    for row in rows:
        price = lookup(row["name"])["price"]
        row["price"] = "{:10.2f}".format(price)
        value = price * row["amount"]
        row["value"] = "{:10.2f}".format(value)
        total_value += value
    
    # Get current user's cash
    cash_rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    cash = cash_rows[0]["cash"]
    total_value += cash
    return render_template("index.html", 
                           stocks=rows, 
                           cash="{:10.2f}".format(cash), 
                           total_value="{:10.2f}".format(total_value))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure the symbol was submitted
        if not request.form.get("symbol"):
            return apology("Missing symbol")
            
        # ensure number of shares was submitted
        if not request.form.get("number_of_shares"):
            return apology("must provide number of shares to buy")
        
        # lookup stock's price
        quote = lookup(request.form.get("symbol"))
        if quote is None:
            return apology("Invalid symbol")
        
        # query database for the amount of cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        # check if the user has enough cash
        amount = int(request.form.get("number_of_shares"))
        value = float(quote["price"]) * amount
        if rows[0]["cash"] < value:
            return apology("Not enough cash")
        
        # Add stock to database
        result = db.execute("INSERT INTO operations (user_id, name, price, amount)" 
                             "VALUES (:user_id, :name, :price, :amount)", 
                             user_id = session["user_id"],
                             name = quote["name"],
                             price = float(quote["price"]),
                             amount = amount)
        if not result:
            return apology("Couldn't add stock")
        
        # Change the amount of cash
        result = db.execute("UPDATE users SET cash = cash - :value WHERE id = :id",
                            value = value,
                            id = session["user_id"])
        if not result:
            return apology("Couldn't update cash")
            
        return redirect(url_for("index"))
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    # Get operations
    rows = db.execute("SELECT * FROM operations WHERE user_id = :id ORDER BY date DESC", id=session["user_id"])
    for row in rows:
        if row["amount"] > 0:
            row["operation"] = "Buy"
        else:
            row["operation"] = "Sell"
        row["value"] = row["price"] * row["amount"]
        
    return render_template("history.html", transactions=rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not myctx.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Allows the user to change his password."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure password was submitted
        old_password = request.form.get("old_password")
        if not old_password:
            return apology("must enter valid password")
        
        # check if user entered valid password
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        if not myctx.verify(old_password, rows[0]["hash"]):
            return apology("invalid password")
        
        # ensure password was confirmed
        new_password = request.form.get("new_password")
        if not request.form.get("confirm_password") or not new_password == request.form.get("confirm_password"):
            return apology("must confirm new password")
            
        # change password
        hash = myctx.hash(new_password)
        result = db.execute("UPDATE users SET hash = :hash", 
                            hash = hash)
        if not result:
            return apology("Couldn't change the password")
        
        # redirect user to home page
        return redirect(url_for("index"))


    #else if user reached route via GET (as by clicking a link or via redirect)
    else:
        # redirect user to login form
        return render_template("change_password.html")

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Missing symbol")
        quote = lookup(request.form.get("symbol"))
        if quote is None:
            return apology("Invalid symbol")
        return render_template("quoted.html", symbol=quote["name"], price=quote["price"])
    elif request.method == "GET":
        return render_template("quote.html")
        
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # forget any user_id
    session.clear()
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
        
        # ensure password was confirmed
        elif not request.form.get("confirm_password") or not request.form.get("password") == request.form.get("confirm_password"):
            return apology("must confirm password")
        
        # register the user
        hash = myctx.hash(request.form.get("password"))
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", 
                            username=request.form.get("username"), hash = hash)
        if not result:
            return apology("Couldn't register the user")
        
        # get and store user's id
        rows = db.execute("SELECT id FROM users WHERE username = :username", username=request.form.get("username"))
        session["user_id"] = rows[0]["id"]
        
        # redirect user to home page
        return redirect(url_for("index"))
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure the symbol was submitted
        symbol = request.form.get("symbol")
        if not symbol or symbol == "Symbol":
            return apology("Must choose stock to sell")
            
        # ensure number of shares was submitted
        amount = int(request.form.get("number_of_shares"))
        if not amount:
            return apology("must enter amount to sell")
            
        # chech if there is enough stock to sell
        rows = db.execute("SELECT name, SUM(amount) AS amount FROM operations " 
                          "WHERE user_id = :id AND name = :name GROUP BY name", 
                          id=session["user_id"],
                          name=symbol)
        if amount > rows[0]["amount"]:
            return apology("not enough stock")
        
        # lookup stock's price
        quote = lookup(request.form.get("symbol"))
        if quote is None:
            return apology("Invalid symbol")
        
        # subtract amount of shares
        result = db.execute("INSERT INTO operations (user_id, name, price, amount)" 
                             "VALUES (:user_id, :name, :price, :amount)", 
                             user_id = session["user_id"],
                             name = quote["name"],
                             price = float(quote["price"]),
                             amount = -amount)
        if not result:
            return apology("Couldn't sell stock")
        
        # add cash
        result = db.execute("UPDATE users SET cash = cash + :value WHERE id = :id",
                            value = float(quote["price"]) * amount,
                            id = session["user_id"])
        if not result:
            return apology("Couldn't get cash")
       
        # redirect user to home page
        return redirect(url_for("index"))
        
        return apology("TODO")
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        # Get the stocks owned by the user
        rows = db.execute("SELECT name FROM operations WHERE user_id = :id GROUP BY name", 
                          id=session["user_id"])
        names = []
        for row in rows:
            names.append(row["name"])
        return render_template("sell.html", names=names)
        
