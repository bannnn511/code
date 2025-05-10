project('C', 'c',
        version : '1.0.0',
        default_options : ['warning_level=3', 'c_std=c11'])

C = executable('C', 'main.c', install : true)
concurrent_counter = executable('concurrent_counter', 'concurrent-counter.c', install : true)
concurrent_linked_list = executable('concurrent_linked_list', 'concurrent-linked-list.c', install : true)
concurrent_queue = executable('concurrent_queue', 'concurrent-queue.c', install : true)
hand_over_hand_linked_list = executable('hand-over-hand-linked-list', 'hand-over-hand-linked-list.c', install : true)
red_black_bst = executable('red-black-bst', 'red-black-bst.c', install : true)
btree = executable('btree', 'btree.c', install : true)
cond_var = executable('cond_var', 'conditional_variable.c', install : true)
reader_writer = executable('reader_writer', 'reader_writer.c', install : true)
fork_join = executable('fork_join', 'fork_join.c', install : true)


# Webserver sources
webserver_sources = [
    'unix/webserver/src/wserver.c',
    'unix/webserver/src/request.c',
    'unix/webserver/src/io_helper.c',
    'unix/webserver/src/threadpool.c',
    'unix/webserver/src/heap.c'
]

# Webserver executable
wserver = executable('wserver',
                     webserver_sources,
                     include_directories : include_directories('unix/webserver/src'),
                     install : true
)

project_sources = [
    'projects/mapreduce/mapreduce.c',
    'projects/mapreduce/task_queue.c',
    'projects/mapreduce/shard.c',
    'projects/mapreduce/threadpool.c',
    'projects/mapreduce/heap.c',
    'projects/mapreduce/kv.c',
    'projects/mapreduce/tests/wordcount.c'
]

mapreduce_test = executable('mapreduce_test',
                            project_sources,
                            #                            c_args : ['-Wall', '-Werror', '-O'],
                            c_args : ['-std=c23', '-Wall', '-Werror', '-Wno-unused-parameter', '-Wno-gnu-statement-expression'],
                            dependencies : [dependency('threads')],
                            install : true)

test('test', C)
