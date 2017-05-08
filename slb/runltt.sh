
PN=memtest

lttng create
lttng enable-channel -u cephtrc --subbuf-size 2097152
lttng enable-event -u -a -c cephtrc
lttng add-context -c cephtrc -u -t pthread_id
ceph osd pool create $PN 100 100
ceph osd pool set $PN size 1
sleep 3
lttng start
rados bench -b 4096 -t 1 -p $PN 2 write --no-cleanup
lttng stop
sleep 3
lttng view > /tmp/lttmemhook_rb4k.out
lttng destroy
