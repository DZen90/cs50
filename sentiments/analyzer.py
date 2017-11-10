import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # Loading positive words
        self.positive_words = set()
        try:
            file = open(positives, "r")
        except:
            print("Could not open: " + positives)
            exit(1)
            
        for line in file:
            # add the line only if the first character is a letter
            if line[:1].isalpha():
                self.positive_words.add(line.rstrip("\n"))
        file.close()
        
        # Loading negative words
        self.negative_words = set()
        try:
            file = open(negatives, "r")
        except:
            print("Could not open: " + negatives)
            exit(1)
            
        for line in file:
            # add the line only if the first character is a letter
            if line[:1].isalpha():
                self.negative_words.add(line.rstrip("\n"))
        file.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # Split text into words
        tokenizer = nltk.tokenize.TweetTokenizer()
        words = tokenizer.tokenize(text)

        score = 0
        
        for word in words:
            if word.lower() in self.positive_words:
                score += 1
            if word.lower() in self.negative_words:
                score -= 1
        
        return score
