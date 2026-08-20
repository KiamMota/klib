#ifndef KPRINT_DEF
#define KPRINT_DEF

#include <string.h>
#include <unistd.h>

static inline void kprint_write(const char *str) {
  size_t len = strlen(str);
  if (len == 0) return;

#if defined(__linux__)
  // Usa STDOUT_FILENO (1) para imprimir na tela (saída padrão)
  // Se realmente precisar forçar a escrita no stdin, mude para STDIN_FILENO (0)
  write(STDOUT_FILENO, str, len);

#endif 
#if defined(_WIN32)
  // Obtém o manipulador da saída padrão (Console)
  // Se realmente precisar forçar no stdin, mude para STD_INPUT_HANDLE
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hStdOut != INVALID_HANDLE_VALUE) {
    DWORD bytesWritten;
    WriteFile(hStdOut, str, (DWORD)len, &bytesWritten, NULL);
  }
#endif
}


// --- Assinaturas Solicitadas ---
void kprintf(const char *format, ...);
void kprintln(const char *format, ...);
void kprint_putc(char c);
void kprint_putd(int n);

// --- Novos Métodos Criados para Substituição do Printf ---

/**
 * @brief Imprime inteiros sem sinal (unsigned int). Mapeado para %u.
 */
void kprint_putu(unsigned int num);

/**
 * @brief Imprime números em formato Hexadecimal minúsculo. Mapeado para %x.
 */
void kprint_puthex(unsigned int num);

/**
 * @brief Imprime variáveis do tipo de tamanho (size_t). Mapeado para %zu.
 */
void kprint_putzu(size_t num);

/**
 * @brief Imprime endereços de ponteiros em formato hexadecimal com o prefixo 0x. Mapeado para %p.
 */
void kprint_putptr(void *ptr);

/**
 * @brief Atalho opcional para imprimir uma string simples. Mapeado para %s.
 */
void kprint_puts(const char *str);

#endif // KPRINT_H


