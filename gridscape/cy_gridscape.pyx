cimport numpy as cnp
from libc.stdlib cimport calloc, free
import os

cdef extern from "client.h":
    ctypedef struct CursesWindow:
        int width
        int height
        void* win
        void* topWin
        void* bottomWin

cdef extern from "gridscape.h":
    int LOG_BUFFER_SIZE

    ctypedef struct Object:
        int id
        char name[12]
        int tier
        int color

    ctypedef struct Tile:
        int id
        Object object

    ctypedef struct Log:
        float episode_return
        float episode_length
        float score

    ctypedef struct LogBuffer:
        Log* logs
        int length
        int idx
    LogBuffer* allocate_logbuffer(int)
    void free_logbuffer(LogBuffer*)
    Log aggregate_and_clear(LogBuffer*)

    ctypedef struct Grid:
        int height
        int width
        Tile** array

    ctypedef struct Player:
        Object object
        int direction
        int type
        int x
        int y

    ctypedef struct Gridscape:
        float* observations
        int* actions
        float* rewards
        unsigned char* terminals
        LogBuffer* log_buffer
        Log log
        Grid* grid
        Player* player
        unsigned int score
        float width
        float height
        unsigned int max_score
        int tick
        int win
        int frameskip

    CursesWindow* make_client(Gridscape* env)
    void close_client(CursesWindow* client)
    void c_render(CursesWindow* client, Gridscape* env)

    void init(Gridscape* env)
    void c_reset(Gridscape* env)
    void c_step(Gridscape* env)

cdef class CyGridscape:
    cdef:
        Gridscape* envs
        CursesWindow* client
        LogBuffer* logs
        int num_envs
        float width
        float height

    def __init__(self, float[:, :] observations, int[:] actions, float[:] rewards, unsigned char[:] terminals, int num_envs, float width, float height, unsigned int max_score, int frameskip):
        self.num_envs = num_envs
        self.client = NULL
        self.envs = <Gridscape*> calloc(num_envs, sizeof(Gridscape))
        self.logs = allocate_logbuffer(LOG_BUFFER_SIZE)
        self.width = width
        self.height = height

        cdef int i
        for i in range(num_envs):
            self.envs[i] = Gridscape(
                observations = &observations[i, 0],
                actions = &actions[i],
                rewards = &rewards[i],
                terminals = &terminals[i],
                log_buffer = self.logs,
                grid = NULL,
                player = NULL,
                width = width,
                height = height,
                max_score = max_score,
                frameskip = frameskip
            )
            init(&self.envs[i])

    def reset(self):
        cdef int i
        for i in range(self.num_envs):
            c_reset(&self.envs[i])

    def step(self):
        cdef int i
        for i in range(self.num_envs):
            c_step(&self.envs[i])

    def render(self):
        cdef Gridscape* env = &self.envs[0]
        if self.client == NULL:
            import os
            cwd = os.getcwd()
            os.chdir(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..")))
            self.client = make_client(env)
            os.chdir(cwd)

        c_render(self.client, env)

    def close(self):
        if self.client != NULL:
            close_client(self.client)
            self.client = NULL

        free(self.envs)
        free(self.logs)

    def log(self):
        cdef Log log = aggregate_and_clear(self.logs)
        return log