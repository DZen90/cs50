import crypt
import sys
import string
import itertools

def main():
    # Check command line arguments
    if len(sys.argv) != 2:
        print("Usage: crack.py hash")
        exit(1)
    
    plain_text = sys.argv[1]
    salt = plain_text[:2]
    
    # Generate all possible strings and checking them
    # https://stackoverflow.com/questions/43119744/python-generate-all-possible-strings-of-length-n
    for length in range(1, 5):
        for sequence in itertools.product(string.ascii_letters, repeat=length):
            key = ''.join(sequence)
            generated_hash = crypt.crypt(key, salt)
            if generated_hash == plain_text:
                print(key)
                exit(0)

if __name__ == "__main__":
    main()