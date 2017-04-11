#ifndef _OUR_MOD_H
#define _OUR_MOD_H


struct commande {
  char nom[25];
  char **param;
  int asynchrone;
  char retour[1024];
};


/* definition of number for ioctl request  */
#define LIST_IO     10
#define FG_IO      11
#define KILL_IO    12
#define WAIT_IO    13
#define MEMINFO_IO  14
#define MODINFO_IO 15


#endif
