#include <linux/netfilter_ipv4.h>
#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/reboot.h>
#include <linux/kmod.h>

static char *envp[] = {"HOME=/", "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL};

void cafe_reboot(void) {
    static char *argv[] = {"/sbin/shutdown", "-r", "now", NULL};
    printk(KERN_INFO "[cafe] Rebotting system\n");
    call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
}

void cafe_reset_iptables(void) {
    //iptables -F
    static char *argv[] = {"/sbin/iptables", "-F", NULL};
    printk(KERN_INFO "[cafe] iptables -F\n");
    call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
}

void cafe_allow_ip(__u32 saddr) {
    //iptables -I INPUT -p tcp ! -s yourIPaddress --dport 22 -j DROP
    static char clientIP[16];
    static char *argv[] = {"/sbin/iptables", "-I", "INPUT", "-p", "tcp", "!", "-s", clientIP, "--dport", "22", "-j", "DROP", NULL};

    cafe_reset_iptables();

    memset(clientIP, 0, sizeof (clientIP));
    snprintf(clientIP, sizeof (clientIP), "%pI4", &saddr);
    printk(KERN_INFO "[cafe] Allowing only this IP: %s\n", clientIP);

    call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
}

//TODO does not work: kernel panic if there is no REJECT rule; does not clear rules

void cafe_clean_xtables(void) {
    struct xt_table table;
    struct xt_target *target;
    //TODO
    return;

    target = xt_find_target(NFPROTO_IPV4, "REJECT", 0);
    if (target == NULL)
        return;

    memcpy((void *) table.name, "filter", sizeof (table.name));
    printk(KERN_INFO "[cafe] %s %s\n", target->name, target->table);

    xt_unregister_target(target);
}

