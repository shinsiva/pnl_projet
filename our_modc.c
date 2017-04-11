/* linux definition */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/string.h>

/* our definition */
#include "our_modc.h"


static struct commande *cmd_list;
static int cmd_cpt = 0;
char retour[1024];

/*
 *ioctl syscall
 */
long device_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);

/*
 *file operations
 */
static struct file_operations fops = {
  .unlocked_ioctl = device_ioctl,
};


/*
 *ioctl functions
 */

static int io_list(int nb) {
  int i, rc = 0;

  for(i = 0; i < nb; i++) {
    pr_info("%d ) Cmd : %s\n", i, cmd_list[i].nom);
    if((rc = copy_to_user(argp->retour, retour, strlen(retour)))) {
      pr_err("Erreur lors de copy_to_user de la struct \"arg\"\n");
      return rc;
    }
  }
  return rc;
}


long device_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {

  struct commande args_cpy;
  struct commande *argp = (struct commande *) arg;
  int rc = 0;
  char retour[1024];
  int i = 0;

  /*
   *Copy from user parameters
   */

  if((rc = copy_from_user(&args_cpy, argp, sizeof(struct commande)))) {
    pr_err("Erreur lors de copy_from_user de la struct \"arg\"\n");
    return rc;
  }
  
  while(args_cpy.param[i] != NULL) {
    strcpy(cmd_list[cmd_cpt].param[i], args_cpy.param[i]);
    i++;
  }

  switch(cmd) {

  case LIST_IO :
    pr_info("Commande List\n");
    strcpy(cmd_list[cmd_cpt].nom, "LIST");
    cmd_cpt++;
    io_list(cmd_cpt);
    break;

  case FG_IO:
    pr_info("Commande Fg\n");
    strcpy(cmd_list[cmd_cpt].nom, "FG");
    cmd_cpt++;
    break;
    
  case KILL_IO:
    pr_info("Commande Kill\n");
    strcpy(cmd_list[cmd_cpt].nom, "KILL");
    cmd_cpt++;
    break;
    
  case WAIT_IO:
    pr_info("Commande Wait\n");
    strcpy(cmd_list[cmd_cpt].nom, "WAIT");
    cmd_cpt++;
    break;
    
  case MEMINFO_IO:
    pr_info("Commande Meminfo\n");
    strcpy(cmd_list[cmd_cpt].nom, "MEMINFO");
    cmd_cpt++;
    break;

  case MODINFO_IO:
    pr_info("Commande Modinfo\n");
    strcpy(cmd_list[cmd_cpt].nom, "MODINFO");
    cmd_cpt++;
    break;
    
  default :
    pr_info("Commande non reconnue !\n");
  }

  return rc;
}

/*
 *module init & exit
 */

static int major;

static int __init module_init(void) {

  pr_info("Insertion du module\n");
  major = register_chrdev(0, "our_mod", &fops);

  cmd_list = kmalloc(10 * sizeof(struct commande), __GFP_WAIT | __GFP_FS);

}

static void __exit module_exit(void) {

  unregister_chrdev(major, "our_mod");
  pr_info("Suppression du module\n");

  kfree(cmd_list);
}

module_init(module_init);
module_exit(module_exit);
