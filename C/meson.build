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
producer_consumer = executable('producer_consumer', 'producer_consumer.c', install : true)
reader_writer = executable('reader_writer', 'reader_writer.c', install : true)
fork_join = executable('fork_join', 'fork_join.c', install : true)
rendezvous = executable('rendezvous', 'rendezvous.c', install : true)
barrier = executable('barrier', 'barrier.c', install : true)

test('test', C)
