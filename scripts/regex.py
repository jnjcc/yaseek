import re

SYMBL_REGEX = re.compile(r'^[\x20-\x2f\x3a-\x40\x5b-\x60\x7b-\x7f]*$')
LATIN_REGEX = re.compile(r'^[\x20-\x7f]*$')
DIGIT_REGEX = re.compile(r'^[0-9$]*$')
ALPHA_REGEX = re.compile(r'^[A-Za-z\-]*$')
ALNUM_REGEX = re.compile(r'^[A-Za-z\-0-9]*$')
PUNCT_REGEX = re.compile(r'\xa1\xa0|\xa1\xa1|\xa1\xae|\xa1\xaf|\xa1\xb0|' \
                          '\xa1\xb1|\xa1\xb2|\xa1\xb3|\xa1\xb4|\xa1\xb5|' \
                          '\xa1\xb8|\xa1\xb9|\xa1\xba|\xa1\xbb|\xa1\xbc|' \
                          '\xa1\xbd|\xa1\xbe|\xa1\xbf|\xa3\xa8|\xa3\xa9|' \
                          '"|\'|\(|\)|\[|\]|<|>|\{|\}')
