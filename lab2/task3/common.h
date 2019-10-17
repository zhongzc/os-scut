
const char *const READER = "./reader";
const int READER_CNT = 10;
const char *const WRITER = "./writer";
const int WRITER_CNT = 10;

const int PERM = 0640;

const char *const FTOK_PATH = ".";
const int FTOK_ID = 42;

const char *const SEM_READABLE_NAME = "/readable";
const int SEM_READABLE_INIT_VALUE = 0;
const char *const SEM_WRITABLE_NAME = "/writable";
const int SEM_WRITABLE_INIT_VALUE = 0;
const char *const SEM_TABLE_MUTEX_NAME = "/table_mutex";
const int SEM_TABLE_MUTEX_INIT_VALUE = 1;

struct waiting_table {
  int writer;
  int reader;
};

struct accessing_table {
  int writer;
  int reader;
};

struct rw_table {
  struct waiting_table waiting_table;
  struct accessing_table accessing_table;
};

const char *R_COME_MSG = "Reader %3d: a reader is coming\n";
const char *R_WAIT_MSG = "Reader %3d: waiting             (waiting-reader: %2d -> %2d, waiting-writer:       %2d)\n";
const char *R_NOTW_MSG = "Reader %3d: no need to wait     (waiting-reader: %2d -> %2d, waiting-writer:       %2d)\n";
const char *R_ACCE_MSG = "Reader %3d: accessing           [viewing-reader: %2d -> %2d, viewing-writer:       %2d]\n";
const char *R_GONE_MSG = "Reader %3d: leaving             [viewing-reader: %2d -> %2d, viewing-writer:       %2d]\n";

const char *W_COME_MSG = "Writer %3d: a writer is coming\n";
const char *W_WAIT_MSG = "Writer %3d: waiting             (waiting-reader:       %2d, waiting-writer: %2d -> %2d)\n";
const char *W_NOTW_MSG = "Writer %3d: no need to wait     (waiting-reader:       %2d, waiting-writer: %2d -> %2d)\n";
const char *W_ACCE_MSG = "Writer %3d: accessing           [viewing-reader:       %2d, viewing-writer: %2d -> %2d]\n";
const char *W_GONE_MSG = "Writer %3d: leaving             [viewing-reader:       %2d, viewing-writer: %2d -> %2d]\n";