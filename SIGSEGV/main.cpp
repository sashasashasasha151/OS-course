#include <iostream>
#include <csetjmp>
#include <bits/types/siginfo_t.h>
#include <csignal>

using namespace std;

jmp_buf rp;
jmp_buf rp2;
void *memory = nullptr;

static void ouch(int sig, siginfo_t *siginfo, void *context) {
    cerr << "can't get info from here, segmentation fault found";
    longjmp(rp2, SIGSEGV);
}


static void action(int sig, siginfo_t *siginfo, void *context) {
    auto *uc = (ucontext_t *) context;

    cerr << "Segmentation fault\n\n";
    cerr << "Registers dump:\n";
    for (int i = 0; i < NGREG; ++i) {
        cerr << "Reg" << i << ": " << (void *) uc->uc_mcontext.gregs[i] << "\n";

    }
    cerr << "\n";

    memory = siginfo->si_addr;
    longjmp(rp, SIGSEGV);
}

int main() {
    struct sigaction act{};
    act.sa_sigaction = &action;
    act.sa_flags = SA_NOMASK, SS_ONSTACK;
    if (sigaction(SIGSEGV, &act, nullptr) < 0) {
        std::cerr << "Sigaction error";
        return 0;
    }

    int fc = setjmp(rp);
    if (fc == 0) {
        int *p = nullptr;
        *p = 42;
    } else {
        act.sa_sigaction = &ouch;
        if (sigaction(SIGSEGV, &act, nullptr) < 0) {
            std::cerr << "Sigaction error";
            return 0;
        }

        cerr << "Memory dump:\n";
        for (auto i = memory - 100000; i != memory + 100000; i+=1000) {
            int fc2 = setjmp(rp2);
            if (fc2 == 0) {
                cerr << "Value in address " << i << ": " << (void *) *reinterpret_cast<unsigned char *>(i) << "\n";
            } else {
                cerr << "\n";
            }
        }
    }

    return 0;
}