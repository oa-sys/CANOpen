#include <math.h>
#include <include/util.h>

/* Credit: http://www.codingalpha.com/convert-hexadecimal-to-decimal-number-c-program/ */
int canopen_Dictionary_hex2dec(int x)
{
    int decimal_number, remainder, count = 0;
    while(x > 0)
    {
        remainder = x % 10;
        decimal_number = decimal_number + remainder * pow(16, count);
        x = x / 10;
        count++;
    }

    return decimal_number;
}

/* Credit: http://www.codingalpha.com/convert-decimal-to-hexadecimal-number-c-program/ */
int canopen_Dictionary_dec2hex(int x)
{
      int hexadecimal_number, remainder, count = 0;
      for(count = 0; x > 0; count++)
      {
            remainder = x % 16;
            hexadecimal_number = hexadecimal_number + remainder * pow(10, count);
            x = x / 16;
      }
      return hexadecimal_number;
}
