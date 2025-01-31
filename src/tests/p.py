import ctypes
from decimal import Decimal


RED = '\033[91m'
GREEN = '\033[92m'
YELLOW = '\033[93m'
BLUE = '\033[94m'
RESET = '\033[0m'
lib = ctypes.CDLL('../src/s21_decimal.so')  



class S21Decimal(ctypes.Structure):
    _fields_ = [("bits", ctypes.c_uint32 * 4)]


lib.wrapper_s21_is_less.argtypes = [S21Decimal, S21Decimal]
lib.wrapper_s21_is_less.restype = ctypes.c_int

lib.wrapper_s21_is_equal.argtypes = [S21Decimal, S21Decimal]
lib.wrapper_s21_is_equal.restype = ctypes.c_int

lib.wrapper_s21_is_greater.argtypes = [S21Decimal, S21Decimal]
lib.wrapper_s21_is_greater.restype = ctypes.c_int

lib.wrapper_s21_is_less_or_equal.argtypes = [S21Decimal, S21Decimal]
lib.wrapper_s21_is_less_or_equal.restype = ctypes.c_int

lib.wrapper_s21_is_greater_or_equal.argtypes = [S21Decimal, S21Decimal]
lib.wrapper_s21_is_greater_or_equal.restype = ctypes.c_int

lib.wrapper_s21_is_not_equal.argtypes = [S21Decimal, S21Decimal]
lib.wrapper_s21_is_not_equal.restype = ctypes.c_int


def decimal_to_s21_decimal(py_decimal):
    try:
        result = S21Decimal((0, 0, 0, 0))
        
        sign = int(py_decimal < 0)
        
        abs_dec_str = format(abs(py_decimal), 'f')
        
        if '.' in abs_dec_str:
            int_part, dec_part = abs_dec_str.split('.')
            int_part = int_part.lstrip('0') or '0'
            
            dec_part = dec_part.rstrip('0')
            
            if not dec_part:
                num_str = int_part
                scale = 0
            else:
                scale = len(dec_part)
                if scale > 28:
                    dec_part = dec_part[:28]
                    scale = 28
                num_str = int_part + dec_part
        else:
            num_str = abs_dec_str.lstrip('0') or '0'
            scale = 0
        
        try:
            num = int(num_str)
            if num >= 2**96:
                print(f"{RED}Number too large for s21_decimal{RESET}")
                return S21Decimal((0, 0, 0, 0))
            
            result.bits[0] = num & 0xFFFFFFFF
            result.bits[1] = (num >> 32) & 0xFFFFFFFF
            result.bits[2] = (num >> 64) & 0xFFFFFFFF
            
            result.bits[3] = (sign << 31) | (scale << 16)
            
        except ValueError as e:
            print(f"{RED}Error converting number: {e}{RESET}")
            return S21Decimal((0, 0, 0, 0))
        
        return result
        
    except Exception as e:
        print(f"{RED}Unexpected error: {e}{RESET}")
        return S21Decimal((0, 0, 0, 0))




def print_decimal_bits(dec):
    print(f"{BLUE}bits[0-2]:{RESET}")
    for i in range(3):
        print(f"  bits[{i}] = {dec.bits[i]:032b} ({dec.bits[i]})")
    print(f"{BLUE}bits[3]:{RESET}")
    print(f"  bits[3] = {dec.bits[3]:032b}")
    print(f"    Sign: {dec.bits[3] >> 31}")
    print(f"    Scale: {(dec.bits[3] >> 16) & 0xFF}")


def test_comparison(a, b, test_name=""):
    try:
        print(f"\n{YELLOW}Testing {test_name}:{RESET}")
        print(f"{YELLOW}Comparing {a} and {b}{RESET}")
        
     
        a_c = decimal_to_s21_decimal(a)
        b_c = decimal_to_s21_decimal(b)
        
        """
        print("\nFirst number representation:")
        print_decimal_bits(a_c)
        print("\nSecond number representation:")
        print_decimal_bits(b_c)
        """
        
        
        # py res
        py_less = a < b
        py_equal = a == b
        py_greater = a > b
        py_less_eq = a <= b
        py_greater_eq = a >= b
        py_not_equal = a != b
        
        # c lib res
        c_less = bool(lib.wrapper_s21_is_less(a_c, b_c))
        c_equal = bool(lib.wrapper_s21_is_equal(a_c, b_c))
        c_greater = bool(lib.wrapper_s21_is_greater(a_c, b_c))
        c_less_eq = bool(lib.wrapper_s21_is_less_or_equal(a_c, b_c))
        c_greater_eq = bool(lib.wrapper_s21_is_greater_or_equal(a_c, b_c))
        c_not_equal = bool(lib.wrapper_s21_is_not_equal(a_c, b_c))
        
        
        print(f"{'Operation':<15} {'Python':<10} {'C lib':<10} {'Match':<10}")
        print("-" * 45)
        tests = [
            ("less", py_less, c_less),
            ("equal", py_equal, c_equal),
            ("greater", py_greater, c_greater),
            ("less_eq", py_less_eq, c_less_eq),
            ("greater_eq", py_greater_eq, c_greater_eq),
            ("not_equal", py_not_equal, c_not_equal)
        ]
        
        all_passed = True
        for op, py_res, c_res in tests:
            match = py_res == c_res
            all_passed &= match
            print(f"{op:<15} {py_res!r:<10} {c_res!r:<10} {f"{GREEN}✓{RESET}" if match else f"{RED}✗{RESET}":<10}")
        
        return all_passed
        
    except Exception as e:
        print(f"{RED}Error in test_comparison: {e}{RESET}")
        return False


if __name__ == '__main__':
    print("=== Decimal Comparison Tests ===")
    
    test_cases = [
        # Basic integer comparisons
        (Decimal("123"), Decimal("456"), "Simple integers"),
        (Decimal("-123"), Decimal("456"), "Mixed signs"),
        (Decimal("-123"), Decimal("-456"), "Negative numbers"),
        
        # Zero comparisons
        (Decimal("0"), Decimal("0"), "Zero equality"),
        (Decimal("-0"), Decimal("0"), "Signed zero"),
        
        # Decimal point numbers
        (Decimal("123.456"), Decimal("123.457"), "Decimal points"),
        (Decimal("-123.456"), Decimal("-123.457"), "Negative decimals"),
        
        # Large numbers
        (Decimal("1234567890123456789012345678"), Decimal("0.12"), "Large vs small"),
        
        # Small numbers
        (Decimal("0.000001"), Decimal("0.000002"), "Small numbers"),
        
        # Equal numbers with different representations
        (Decimal("1.23000"), Decimal("1.23"), "Different precision"),
        
        # Edge cases
        (Decimal("0.0"), Decimal("-0.0"), "Zero representations"),
        (Decimal("1"), Decimal("1.00000"), "Integer vs decimal")
    ]
    
    all_tests_passed = True
    for a, b, name in test_cases:
        all_tests_passed &= test_comparison(a, b, name)
    
    print("\n=== Test Summary ===")
    print(f"{GREEN}All tests passed!{RESET}" if all_tests_passed else f"{RED}Some tests failed!{RESET}")
