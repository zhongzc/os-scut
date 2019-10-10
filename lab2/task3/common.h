
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