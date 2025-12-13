#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_SIZE 10000

typedef struct {
    int src_state;
    int read_symbol;
    int dst_state;
    int write_symbol;
    char direction;
    int input_index;
} Transition;

typedef struct {
    int *tape;
    int state;
    int head;
    int left;
    int right;
} Config;

Transition *trans = NULL;
int trans_count = 0, trans_capacity = 0;

char **symbols = NULL, *blank_sym = NULL;
int M, N, K, TAPE_SIZE = 0, QUEUE_SIZE = 0;

void trim_newline(char *s) {
    size_t l = strlen(s);
    if (l > 0 && s[l-1] == '\n') {
        s[l-1] = '\0';
    }
}

void decode_tm(char *code) {
    int len = (int)strlen(code), start = 0, end = len;

    if (len >= 3 && code[0]=='1' && code[1]=='1' && code[2]=='1') {
        start = 3;
    }
    if (len >= 3 && code[len-3]=='1' && code[len-2]=='1' && code[len-1]=='1') {
        end = len-3;
    }

    int pos = start;
    while (pos < end) {
        int next = pos;
        while (next < end - 1) {
            if (code[next]=='1' && code[next+1]=='1') {
                break;
            }
            next++;
        }
        if (next == pos) {
            pos = next + 2;
            continue;
        }
        int token_len = next - pos;
        char *token = (char*)malloc(token_len + 1);
        memcpy(token, code + pos, token_len);
        token[token_len] = '\0';

        int vals[5] = {0,0,0,0,0}, vi = 0, cnt0 = 0;
        for (int i = 0; i < token_len; i++) {
            if (token[i] == '0') {
                cnt0++;
            } else if (token[i] == '1') {
                if (vi < 5) {
                    vals[vi] = cnt0;
                    vi++;
                }
                cnt0 = 0;
            } else {
            }
        }
        if (vi < 5) {
            vals[vi] = cnt0;
            vi++;
        }

        if (vi == 5) {
            if (trans_count >= trans_capacity) {
                trans_capacity = trans_capacity == 0 ? 1000 : trans_capacity * 2;
                trans = (Transition*)realloc(trans, trans_capacity * sizeof(Transition));
                if (!trans) {
                    fprintf(stderr, "Memory reallocation failed\n");
                    free(token);
                    exit(1);
                }
            }
            Transition t;
            t.src_state = vals[0];
            t.read_symbol = vals[1];
            t.dst_state = vals[2];
            t.write_symbol = vals[3];
            int r = vals[4];
            if (r == 1) {
                t.direction = 'R';
            } else {
                t.direction = 'L';
            }
            t.input_index = trans_count;
            trans[trans_count++] = t;
        }
        free(token);

        if (next < end - 1 && code[next]=='1' && code[next+1]=='1') {
            pos = next + 2;
        } else {
            pos = next;
        }
    }
}

int is_deterministic() {
    int maxState = N + 2, maxSym = M + 2;
    int *map = (int*)malloc((maxState) * (maxSym) * sizeof(int));
    if (!map) {
        fprintf(stderr, "Memory error\n");
        exit(1);
    }
    for (int i = 0; i < maxState*maxSym; i++) {
        map[i] = -1;
    }

    for (int i = 0; i < trans_count; i++) {
        int s = trans[i].src_state, sym = trans[i].read_symbol;
        if (s < 1 || s > N || sym < 1 || sym > M+1) {
            continue;
        }
        int idx = s * maxSym + sym;
        if (map[idx] == -1) {
            map[idx] = i;
        } else {
            free(map);
            return 0;
        }
    }
    free(map);
    return 1;
}

void print_tape_content(int *tape, int left, int right) {
    int first = -1, last = -1;
    for (int i = left; i <= right; i++) {
        if (tape[i] != (M+1)) {
            if (first == -1) {
                first = i;
            }
            last = i;
        }
    }
    if (first == -1) {
        fprintf(stdout, "\n");
        return;
    }
    for (int i = first; i <= last; i++) {
        int sym = tape[i];
        if (sym == (M+1)) {
            fprintf(stdout, "%s", blank_sym);
        } else {
            fprintf(stdout, "%s", symbols[sym-1]);
        }
        if (i < last) {
            fprintf(stdout, " ");
        }
    }
    fprintf(stdout, "\n");
}

void simulate_deterministic(int *init_tape, int init_len) {

    int *tape = (int*)malloc(TAPE_SIZE * sizeof(int));
    if (!tape) {
        fprintf(stderr,"alloc fail\n");
        exit(1);
    }
    for (int i = 0; i < TAPE_SIZE; i++) {
        tape[i] = M+1;
    }

    int offset = TAPE_SIZE / 2;
    for (int i = 0; i < init_len; i++) {
        tape[offset + i] = init_tape[i];
    }

    int state = 1, head = offset, left = offset, right = offset + init_len - 1;
    if (init_len == 0) {
        left = right = offset;
    }

    int maxState = N + 2, maxSym = M + 2;
    int *tmap = (int*)malloc(maxState * maxSym * sizeof(int));
    if (!tmap) {
        fprintf(stderr,"alloc fail\n");
        exit(1);
    }
    for (int i = 0; i < maxState*maxSym; i++) {
        tmap[i] = -1;
    }
    for (int i = 0; i < trans_count; i++) {
        int s = trans[i].src_state, sym = trans[i].read_symbol;
        if (s >= 1 && s <= N && sym >= 1 && sym <= M+1) {
            tmap[s * maxSym + sym] = i;
        }
    }

    while (1) {
        if (state == 2) {

            print_tape_content(tape, left, right);
            break;
        }
        if (head < 0 || head >= TAPE_SIZE) {

            break;
        }
        int cur_sym = tape[head];
        if (cur_sym < 1 || cur_sym > M+1) {
            cur_sym = M+1;
        }
        int idx = tmap[state * maxSym + cur_sym];
        if (idx == -1) {

            break;
        }

        tape[head] = trans[idx].write_symbol;
        if (trans[idx].direction == 'R') {
            head++;
        } else {
            head--;
        }
        if (head < left) {
            left = head;
        }
        if (head > right) {
            right = head;
        }
        state = trans[idx].dst_state;
    }

    free(tmap);
    free(tape);
}

void simulate_nondeterministic(int *init_tape, int init_len) {
    Config *queue = (Config*)malloc(sizeof(Config) * (QUEUE_SIZE+5));
    if (!queue) {
        fprintf(stderr,"alloc fail\n");
        exit(1);
    }
    int qfront = 0, qrear = 0;

    queue[qrear].tape = (int*)malloc(TAPE_SIZE * sizeof(int));
    if (!queue[qrear].tape) {
        fprintf(stderr,"alloc fail\n");
        exit(1);
    }
    for (int i = 0; i < TAPE_SIZE; i++) {
        queue[qrear].tape[i] = M+1;
    }
    int offset = TAPE_SIZE / 2;
    for (int i = 0; i < init_len; i++) {
        queue[qrear].tape[offset + i] = init_tape[i];
    }
    queue[qrear].state = 1;
    queue[qrear].head = offset;
    queue[qrear].left = offset;
    queue[qrear].right = offset + init_len - 1;
    if (init_len == 0) {
        queue[qrear].left = queue[qrear].right = offset;
    }
    qrear++;

    int found = 0;
    while (qfront < qrear && qfront < QUEUE_SIZE && !found) {
        Config cur = queue[qfront++];
        if (cur.state == 2) {

            print_tape_content(cur.tape, cur.left, cur.right);
            found = 1;

        } else {
            if (cur.head < 0 || cur.head >= TAPE_SIZE) {

            } else {
                int cur_sym = cur.tape[cur.head];
                if (cur_sym < 1 || cur_sym > M+1) {
                    cur_sym = M+1;
                }

                for (int t = 0; t < trans_count; t++) {
                    if (trans[t].src_state == cur.state && trans[t].read_symbol == cur_sym) {
                        if (qrear >= QUEUE_SIZE) {
                            continue;
                        }

                        queue[qrear].tape = (int*)malloc(TAPE_SIZE * sizeof(int));
                        if (!queue[qrear].tape) {
                            fprintf(stderr,"alloc fail\n");
                            exit(1);
                        }
                        memcpy(queue[qrear].tape, cur.tape, TAPE_SIZE * sizeof(int));

                        queue[qrear].tape[cur.head] = trans[t].write_symbol;
                        queue[qrear].state = trans[t].dst_state;
                        if (trans[t].direction == 'R') {
                            queue[qrear].head = cur.head + 1;
                        } else {
                            queue[qrear].head = cur.head - 1;
                        }

                        queue[qrear].left = cur.left;
                        queue[qrear].right = cur.right;
                        if (queue[qrear].head < queue[qrear].left) {
                            queue[qrear].left = queue[qrear].head;
                        }
                        if (queue[qrear].head > queue[qrear].right) {
                            queue[qrear].right = queue[qrear].head;
                        }
                        qrear++;
                    }
                }
            }
        }
        free(cur.tape);
    }

    for (int i = qfront; i < qrear; i++) {
        if (queue[i].tape) {
            free(queue[i].tape);
        }
    }
    free(queue);
}

int main() {
    if (scanf("%d %d %d", &N, &M, &K) != 3) {
        return 0;
    }

    symbols = (char**)malloc(M * sizeof(char*));
    if (!symbols) {
        fprintf(stderr,"alloc fail\n");
        return 0;
    }
    for (int i = 0; i < M; i++) {
        symbols[i] = (char*)malloc(64 * sizeof(char));
        if (!symbols[i]) {
            fprintf(stderr,"alloc fail\n");
            return 0;
        }
        if (scanf("%63s", symbols[i]) != 1) {
            return 0;
        }
    }
    
    blank_sym = (char*)malloc(64 * sizeof(char));
    if (!blank_sym) {
        fprintf(stderr,"alloc fail\n");
        return 0;
    }
    if (scanf("%63s", blank_sym) != 1) {
        return 0;
    }
    
    int code_capacity = START_SIZE, occupied = 0;
    char *code = (char*)malloc(code_capacity);
    if (!code) {
        fprintf(stderr,"alloc fail\n");
        return 0;
    }

    int c;
    while ((c = getchar()) != EOF && (c == ' ' || c == '\n' || c == '\t' || c == '\r'));

    if (c != EOF) {
        do {
            if (occupied + 2 >= code_capacity) {
                code_capacity *= 2;
                code = (char*)realloc(code, code_capacity);
                if (!code) {
                    fprintf(stderr,"alloc fail\n");
                    return 0;
                }
            }
            code[occupied++] = c;
        } while ((c = getchar()) != EOF && c != ' ' && c != '\n' && c != '\t' && c != '\r');
    }

    code[occupied] = '\0';

    int *init_tape = (int*)malloc((K+2) * sizeof(int));
    if (!init_tape) {
        fprintf(stderr,"alloc fail\n");
        free(code);
        return 0;
    }
    for (int i = 0; i < K; i++) {
        char symbuf[64];
        if (scanf("%63s", symbuf) != 1) {
            symbuf[0] = '\0';
        }

        int found = 0;
        for (int j = 0; j < M; j++) {
            if (strcmp(symbuf, symbols[j]) == 0) {
                init_tape[i] = j + 1;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (strcmp(symbuf, blank_sym) == 0) {
                init_tape[i] = M + 1;
                found = 1;
            } else {
                init_tape[i] = M + 1;
            }
        }
    }

    TAPE_SIZE = 300000;

    if (K > 1000) {
        TAPE_SIZE = K * 300;
    }

    QUEUE_SIZE = 200000;
    
    decode_tm(code);

    int det = is_deterministic();
    fprintf(stdout, "%c\n", det ? 'D' : 'N');

    if (det) {
        simulate_deterministic(init_tape, K);
    } else {
        simulate_nondeterministic(init_tape, K);
    }

    if (trans) {
        free(trans);
    }
    free(code);
    free(init_tape);
    free(blank_sym);
    for (int i = 0; i < M; i++) {
        free(symbols[i]);
    }
    free(symbols);
    
    return 0;
}