#include "kfile.h"
#include "kprint.h"
#include "kstring.h"

#include <stdio.h>

int main(void) {
  

  KFile *f = kfile_init("myfile.txt");
  
  kprintln("hello!");

  if (!f) {
    kprintf("falha ao criar KFile\n");
    return 1;
  }
  kprint_write("olá! estou escrevendo no linux!");

  if (!kfile_create(f)) {
    kprintf("falha ao criar arquivo\n");
    kfile_close(f);
    return 1;
  }

  kfile_close(f);

  f = kfile_open("myfile.txt", KFILE_READ_WRITE);

  if (!f) {
    kprintf("falha ao abrir arquivo\n");
    return 1;
  }

  const char *msg = "Olá, Kiam!";

  if (!kfile_write(f, "olá")) {
    kprintf("falha ao escrever\n");
    kfile_close(f);
    return 1;
  }

  kfile_close(f);

  f = kfile_open("myfile.txt", KFILE_READ);

  if (!f) {
    kprintf("falha ao reabrir arquivo\n");
    return 1;
  }


  KString file_msg = kstring_init();
  if (!kfile_read(f, &file_msg, f->stat.size)) {
    kprintf("falha ao ler\n");
    kfile_close(f);
    return 1;
  }

  kprintf("conteudo: %s\n", kstring_cstr(&file_msg));
  kprintf("tamanho: %zu bytes\n", f->stat.size);

  kfile_close(f);

  return 0;
}
