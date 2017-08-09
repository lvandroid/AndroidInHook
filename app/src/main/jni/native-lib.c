#include <jni.h>
#include <sys/socket.h>
#include "inlineHook.h"
#include "android_log.h"
#include <netdb.h>
#include <unistd.h>

void (*old_log)(const char *) = NULL;

int (*old_socket)(int, int, int)=NULL;

int (*old_impl_recvmmsg)(int, struct mmsghdr *, unsigned int, int, const struct timespec *)=NULL;

int (*old_impl_recvmsg)(int, struct msghdr *, int)=NULL;

int (*__old_impl_accept)(int, struct sockaddr *, socklen_t *)=NULL;

int (*__old_impl_accept4)(int, struct sockaddr *, socklen_t *, int)=NULL;

int (*__old_impl_bind)(int, const struct sockaddr *, int)=NULL;

int (*__old_impl_getsockname)(int, struct sockaddr *, socklen_t *)=NULL;

int (*__old_impl_getsockopt)(int, int, int, void *, socklen_t *)=NULL;

int (*__old_impl_listen)(int, int)=NULL;

int (*__old_impl_sendmmsg)(int, const struct mmsghdr *, unsigned int, int)=NULL;

int (*__old_impl_sendmsg)(int, const struct msghdr *, int)=NULL;

int (*__old_impl_setsockopt)(int, int, int, const void *, socklen_t)=NULL;

int (*__old_impl_shutdown)(int, int)=NULL;

int (*__old_impl_socketpair)(int, int, int, int *)=NULL;

int (*__old_impl_getaddrinfo)(const char *hostname, const char *servname,
                              const struct addrinfo *hints, struct addrinfo **res)=NULL;

int (*__old_impl_read)(int __fd, void *__buf, size_t __count)=NULL;

void origin_log(const char *str) {
    LOGE("%s", str);
    char buf[10];
    buf[0] = '1';
    buf[1] = '2';
    buf[2] = '3';
    buf[3] = 0;
    LOGE("%s", buf);
}

void new_log(const char *str) {
    LOGE("hooked");
//    old_log("log hooked");
}

int __za_socket(int family, int type, int protocol) {
    int ret = -1;
    LOGD("socket function ->>>");
    ret = old_socket(family, type, protocol);
    LOGD("sock:%d, family:%d, type:%d, protocol:%d", ret, family, type, protocol);
    return ret;
}

static int
__za_recvmmsg(int a, struct mmsghdr *b, unsigned int c, int d, const struct timespec *e) {
    int ret = -1;
//    print_msg_info(b);
    LOGD("recvmmsg function->>>>>");
    return ret;
}

int __za_recvmsg(int a, struct msghdr *b, int c) {
    int ret = -1;
    LOGD("recvmsg function ->>>");
    ret = old_impl_recvmsg(a, b, c);
    return ret;
}

static int __za_accept(int a, struct sockaddr *b, socklen_t *c) {
    int ret = -1;
    LOGD("accept function->>>");
    ret = __old_impl_accept(a, b, c);
    return ret;
}

//static int __za_accept4(int a, struct sockaddr *b, socklen_t *c, int d) {
//    int ret = -1;
//    LOGD("accept4 function->>>");
//    if (NULL != __old_impl_accept4) {
//        ret = __old_impl_accept4(a, b, c, d);
//    }
//    return ret;
//}

static int __za_bind(int a, const struct sockaddr *b, int c) {
    int ret = -1;
    LOGD("bind function->>>");
    ret = __old_impl_bind(a, b, c);
    return ret;
}

static int __za_getsockname(int a, struct sockaddr *b, socklen_t *c) {
    int ret = -1;
    LOGD("getsockname function->>>");
    ret = __old_impl_getsockname(a, b, c);
    return ret;
}

static int __za_getsockopt(int a, int b, int c, void *d, socklen_t *e) {
    int ret = -1;
    LOGD("getsockopt function ->>>");
    ret = __old_impl_getsockopt(a, b, c, d, e);
    return ret;
}

static int __za_listen(int a, int b) {
    int ret = -1;
    LOGD("listen function ->>>");
    ret = __old_impl_listen(a, b);
    return ret;
}


static int __za_sendmsg(int a, const struct msghdr *b, int c) {
    int ret = -1;
    LOGD("sendmsg function ->>>");
    ret = __old_impl_sendmsg(a, b, c);
    return ret;
}

static int __za_setsockopt(int a, int b, int c, const void *d, socklen_t e) {
    int ret = -1;
    LOGD("setsockopt function ->>>");
    ret = __old_impl_setsockopt(a, b, c, d, e);
    return ret;
}

static int __za_shutdown(int a, int b) {
    int ret = -1;
    LOGD("shutdown function ->>>");
    ret = __old_impl_shutdown(a, b);
    return ret;
}

static int __za_socketpair(int a, int b, int c, int *d) {
    int ret = -1;
    LOGD("socketpair function ->>>");
    ret = __old_impl_socketpair(a, b, c, d);
    return ret;
}

int __za_getaddrinfo(const char *hostname, const char *servname,
                     const struct addrinfo *hints, struct addrinfo **res) {
    int ret = -1;
    LOGD("getaddrinfo function ->>>");
    ret = __old_impl_getaddrinfo(hostname, servname, hints, res);
    return ret;
}

int __za_read(int __fd, void *__buf, size_t __count) {
    int ret = -1;
    LOGD("read function===>>>");
    ret = __old_impl_read(__fd, __buf, __count);
    return ret;
}

int hook() {
    if (registerInlineHook((uint32_t) origin_log, (uint32_t) new_log, (uint32_t **) &old_log)
        != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) origin_log) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int hooksocket() {
    if (registerInlineHook((uint32_t *) socket, (uint32_t *) __za_socket,
                           (uint32_t **) &old_socket) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t *) socket) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hookrsvmsg() {
    if (registerInlineHook((uint32_t) recvmsg, (uint32_t) __za_recvmsg,
                           (uint32_t **) &old_impl_recvmsg) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) recvmsg) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hookaccept() {
    if (registerInlineHook((uint32_t) accept, (uint32_t) __za_accept,
                           (uint32_t **) &__old_impl_accept) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) accept) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hookbind() {
    if (registerInlineHook((uint32_t) bind, (uint32_t) __za_bind,
                           (uint32_t **) &__old_impl_bind) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) bind) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hookgetsocketname() {
    if (registerInlineHook((uint32_t) getsockname, (uint32_t) __za_getsockname,
                           (uint32_t **) &__old_impl_getsockname) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) getsockname) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hookgetsockopt() {
    if (registerInlineHook((uint32_t) getsockopt, (uint32_t) __za_getsockopt,
                           (uint32_t **) &__old_impl_getsockopt) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) getsockopt) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}


int hooklisten() {
    if (registerInlineHook((uint32_t) listen, (uint32_t) __za_listen,
                           (uint32_t **) &__old_impl_listen) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) listen) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hooksendmsg() {
    if (registerInlineHook((uint32_t) sendmsg, (uint32_t) __za_sendmsg,
                           (uint32_t **) &__old_impl_sendmsg) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) sendmsg) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hooksetsockopt() {
    if (registerInlineHook((uint32_t) setsockopt, (uint32_t) __za_setsockopt,
                           (uint32_t **) &__old_impl_setsockopt) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) setsockopt) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hookshutdown() {

    if (registerInlineHook((uint32_t) shutdown, (uint32_t) __za_shutdown,
                           (uint32_t **) &__old_impl_shutdown) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) shutdown) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hooksockpair() {
    if (registerInlineHook((uint32_t) socketpair, (uint32_t) __za_socketpair,
                           (uint32_t **) &__old_impl_socketpair) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) socketpair) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hookgetaddr() {
    if (registerInlineHook((uint32_t) getaddrinfo, (uint32_t) __za_getaddrinfo,
                           (uint32_t **) &__old_impl_getaddrinfo) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) getaddrinfo) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}

int hookread() {
    if (registerInlineHook((uint32_t) read, (uint32_t) __za_read,
                           (uint32_t **) &__old_impl_read) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) read) != ELE7EN_OK) {
        return -1;
    }
    return 0;
}


int unHook() {
    if (inlineUnHook((uint32_t) read) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}


jstring
Java_com_andr0day_androidinlinehook_MainActivity_stringFromJNI(JNIEnv *env, jobject obj) {
//    origin_log("test1");
//    int ret = hook();
//    LOGE("%d", ret);
//    origin_log("test2");
//    unHook();
//    origin_log("test3");
    int hook_getaddr = hookgetaddr();
//    int hook_socket = hooksocket();
//    int hook_resvmsg = hookrsvmsg();
//    int hook_accept = hookaccept();
//    int hook_bind = hookbind();
//    int hook_getsockname = hookgetsocketname();
//    int hook_getsockopt = hookgetsockopt();
//    int hook_listen = hooklisten();
//    int hook_sendmsg = hooksendmsg();
//    int hook_setsockopt = hooksetsockopt();
//    int hook_shutdown = hookshutdown();
//    int hook_sockpair = hooksockpair();

    int hook_read = hookread();
//
    LOGW("hooksocket:%d,hookrecvmsg:%d, hookaccept:%d, hookbind:%d,hookgetsocketname:%d,hookgetsockopt:%d,"
                 "hooklisten:%d, hooksetsockopt:%d, hookshutdown:%d, hooksockpair:%d, hookread:%d",
//         hook_socket,
//         hook_resvmsg,
//         hook_accept,
//         hook_bind,
//         hook_getsockname,
//         hook_getsockopt,
//         hook_listen,
//         hook_setsockopt,
//         hook_shutdown,
//         hook_sockpair,
         hook_read
    );
//    socket(0, 0, 0);
//    recvmsg(0, NULL, NULL);
//    accept(0, NULL, NULL);
//    bind(0, NULL, NULL);
//    getsockname(0, NULL, NULL);
//    getsockopt(0, 0, 0, NULL, NULL);
//    listen(0, 0);
//    sendmsg(0, NULL, 0);
//    setsockopt(0, 0, 0, NULL, NULL);
//    shutdown(0, 0);
//    socketpair(0, 0, 0, 0);
//    int hook_getaddr= hookgetaddr();
//    LOGD("getaddr:%d",hook_getaddr);
//    getaddrinfo(NULL,NULL,NULL,NULL);
    return (*env)->NewStringUTF(env, "WTF");
}

JNIEXPORT void JNICALL
Java_com_andr0day_androidinlinehook_MainActivity_unhook(JNIEnv *env, jobject instance) {

    // TODO
    int ret = unHook();
    LOGD("unhook:%d", ret);
}