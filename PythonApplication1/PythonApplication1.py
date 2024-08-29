
import random
import sympy.ntheory as nt

def is_prime(num):
    if num < 2:
        return False
    for i in range(2, int(num**0.5) + 1):
        if num % i == 0:
            return False
    return True

def has_consecutive_ones_or_zeros(num):
    # Check if the number has more than 3 consecutive 1s or 0s
    count = 1
    prevBit = num & 1
    num >>= 1
    c = 1
    while c <= 64:
        curBit = num & 1
        if curBit == prevBit:
            count += 1
            if count > 3:
                return True
        else:
            count = 1
            prevBit = curBit
        num >>= 1
        c += 1
    return False
    
def has_unique_hex_digits(num):
    # Check if the number has unique HEX digits
    seen_digits = set()
    while num > 0:
        digit = num & 0xF + (1 << 4)   # Extract the last 4 bits (digit)
        if digit in seen_digits:
            return False
        seen_digits.add(digit)
        num >>= 5  # Shift right by 4 bits
    return True   

def has_no_same_siblings(num):
    # Check if the number has unique HEX digits
    seen_digits = set()
    prevDigit = num & 0xF;
    num >>= 4
    while num > 0:
        digit = num & 0xF  # Extract the last 4 bits (digit)
        if digit == prevDigit:
            return False
        prevDigit = digit
        num >>= 4  # Shift right by 4 bits
    return True  

def generate_prime_numbers(N):
    prime_numbers = []
    c = 0
    while c < N:
        candidate = random.getrandbits(64)
        candidate = nt.nextprime(candidate);
        if has_unique_hex_digits(candidate) and not has_consecutive_ones_or_zeros(candidate) and has_no_same_siblings(candidate):
            print(hex(candidate))
            c += 1
    return prime_numbers

N = 10  # Change this to the desired number of prime numbers
prime_numbers = generate_prime_numbers(N)
    