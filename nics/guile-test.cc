#include <pthread.h>
#include <guile/gh.h>
#include <iostream>
#include <unistd.h>

void *thread(void *) {
  cerr << "thread started " << getpid() << endl;
  gh_eval_str("(display \"test\")(newline)");
  return NULL;
}

void real_main(int, char **) {
  pthread_t t1, t2, t3;
  for(;;) {
    pthread_create(&t1, NULL, thread, NULL);
    pthread_create(&t2, NULL, thread, NULL);
    pthread_create(&t3, NULL, thread, NULL);
    //sleep(1);
    pthread_join(t3, NULL);
    sleep(1);
  }
}

int main(int argc, char *argv[]) {
  scm_init_guile();
  gh_enter(argc, argv, real_main);

  return 0;
}
