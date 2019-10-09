static const int total_product = 180;

const char *const PRODUCER = "./producer";
const int PRODUCER_CNT = 2;
const int PRODUCER_TIMES = total_product / PRODUCER_CNT;

const char *const CONSUMER = "./consumer";
const int CONSUMER_CNT = 3;
const int CONSUMER_TIMES = total_product / CONSUMER_CNT;

const int PERM = 0640;

const char *const FTOK_PATH = ".";
const int FTOK_ID = 42;

const char *const SEM_QUEUE_LEN_NAME = "/queue";
const int SEM_QUEUE_LEN_INIT_VALUE = 0;

const char *const SEM_QUEUE_MUTEX_NAME = "/queue_mutex";
const int SEM_QUEUE_MUTEX_INIT_VALUE = 1;

const int BUF_SIZE = 64 - sizeof(int) * 3;
struct queue {
    int size;
    int pro_idx;
    int con_idx;
    char buf[BUF_SIZE];
};
