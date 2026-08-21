#include "kprint.h"
#include <stdarg.h>
#include <stddef.h> // Necessário para size_t e uintptr_t
#include <stdint.h>

// 1. Imprime um único caractere
void kprint_putc(char c) {
  char buff[2] = {c, '\0'};
  kprint_write(buff);
}

// 2. Imprime inteiros com sinal (%d) - CORRIGIDO
void kprint_putd(int n) {
  char buff[32];
  int i = 0;

  if (n == 0) {
    kprint_putc('0');
    return;
  }

  // Tratamento correto do sinal usando o int original
  unsigned int num = n;
  if (n < 0) {
    kprint_putc('-');
    num = -n; // Converte para positivo de forma segura no tipo unsigned
  }

  while (num > 0) {
    buff[i++] = (num % 10) + '0';
    num /= 10;
  }

  char invert[32];
  int j = 0;
  while (i > 0) {
    invert[j++] = buff[--i];
  }
  invert[j] = '\0';
  kprint_write(invert);
}

// 3. Imprime inteiros sem sinal (%u)
void kprint_putu(unsigned int num) {
  char buff[32];
  int i = 0;

  if (num == 0) {
    kprint_putc('0');
    return;
  }

  while (num > 0) {
    buff[i++] = (num % 10) + '0';
    num /= 10;
  }

  char invert[32];
  int j = 0;
  while (i > 0) {
    invert[j++] = buff[--i];
  }
  invert[j] = '\0';
  kprint_write(invert);
}

// 4. Imprime números em formato Hexadecimal (%x)
void kprint_puthex(unsigned int num) {
  char buff[32];
  int i = 0;
  const char hex_digits[] = "0123456789abcdef";

  if (num == 0) {
    kprint_putc('0');
    return;
  }

  while (num > 0) {
    buff[i++] = hex_digits[num % 16];
    num /= 16;
  }

  char invert[32];
  int j = 0;
  while (i > 0) {
    invert[j++] = buff[--i];
  }
  invert[j] = '\0';
  kprint_write(invert);
}

// 5. Imprime o tamanho de memória (%zu)
void kprint_putzu(size_t num) {
  char buff[64]; // Buffer maior para arquiteturas 64 bits
  int i = 0;

  if (num == 0) {
    kprint_putc('0');
    return;
  }

  while (num > 0) {
    buff[i++] = (num % 10) + '0';
    num /= 10;
  }

  char invert[64];
  int j = 0;
  while (i > 0) {
    invert[j++] = buff[--i];
  }
  invert[j] = '\0';
  kprint_write(invert);
}

// 6. Imprime endereços de ponteiros (%p)
void kprint_putptr(void *ptr) {
  if (ptr == NULL) {
    kprint_write("(nil)");
    return;
  }

  // Imprime o prefixo padrão de ponteiros hexadecimais
  kprint_write("0x");

  // Converte o ponteiro para um inteiro numérico do tamanho correto do sistema
  uintptr_t num = (uintptr_t)ptr;
  char buff[64];
  int i = 0;
  const char hex_digits[] = "0123456789abcdef";

  while (num > 0) {
    buff[i++] = hex_digits[num % 16];
    num /= 16;
  }

  char invert[64];
  int j = 0;
  while (i > 0) {
    invert[j++] = buff[--i];
  }
  invert[j] = '\0';
  kprint_write(invert);
}

// Motor interno do Printf completo
static void __kprintf__(const char *format, va_list args) {
    for (const char *p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 's': {
                    char *s = va_arg(args, char *);
                    if (!s) s = "(null)"; // Proteção contra ponteiros nulos
                    kprint_write(s);
                    break;
                }
                case 'c': {
                    // char é promovido para int na pilha de argumentos variáveis
                    char c = (char)va_arg(args, int);
                    kprint_putc(c);
                    break;
                }
                case 'd':
                case 'i': { // %i também é interpretado como inteiro com sinal
                    int n = va_arg(args, int);
                    kprint_putd(n);
                    break;
                }
                case 'u': {
                    unsigned int u = va_arg(args, unsigned int);
                    kprint_putu(u);
                    break;
                }
                case 'x': {
                    unsigned int x = va_arg(args, unsigned int);
                    kprint_puthex(x);
                    break;
                }
                case 'z': {
                    if (*(p + 1) == 'u') {
                        p++; // Avança o ponteiro consumindo o 'u'
                        size_t zu = va_arg(args, size_t);
                        kprint_putzu(zu);
                    }
                    break;
                }
                case 'p': {
                    void *ptr = va_arg(args, void *);
                    kprint_putptr(ptr);
                    break;
                }
                case '%': {
                    kprint_putc('%');
                    break;
                }
                default: {
                    // Caso digitem algo inválido como %y, imprime o '%' e o caractere original
                    kprint_putc('%');
                    kprint_putc(*p);
                    break;
                }
            }
        } else {
            kprint_putc(*p);
        }
    }
}

void kprintln(const char *format, ...) {
    va_list args;
    va_start(args, format);
    __kprintf__(format, args);
    va_end(args);
    kprint_putc('\n');
}

void kprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    __kprintf__(format, args);
    va_end(args);
}
