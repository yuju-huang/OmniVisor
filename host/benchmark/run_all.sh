// Experiment 1: Memcached upperbound 

// Start a VM running LC tasks
host $ sudo ./run_lc_vm.sh 16384 2 16384

// Start memcached server
LC-VM1 $ cd ~/OmniVisor/guest/benchmark/memcached
LC-VM1 $ ./run.sh 8192

// Fill memcached server with 8G
host $ /home/gic4107/mutated/out/bin/mutated_memcache -k 22 -z 2000000 -v 4096 -u 9 127.0.0.1:4441 -s 120 40000 -n 16

// Check memory usage of memcached server, should ~= 8G
LC-VM1 $ memcstat --servers=localhost:11211 | grep " bytes:" | awk '{print $2}' # 7870132143

// Do read request and record results
host $ /home/gic4107/mutated/out/bin/mutated_memcache -k 22 -z 2000000 -v 4096 -u 0.1 127.0.0.1:4441 -s 120 40000 -n 16


// Experiment 2: Interference study
VM_TOTAL_MEM=16384
VM_NUM_VCPU=2

HADOOP_PATH=

start_mapreduce() {
$1 as target hostname
$ $HADOOP_PATH/sbin/start-all.sh
$ $HADOOP_PATH/bin/hadoop dfs -rmr output*
$ $HADOOP_PATH/bin/hadoop dfs jar share/hadoop/mapreduce/hadoop-mapreduce-examples-3.2.1.jar wordcount wiki_dataset output
}

// Experiment 2-1: Baseline, all local memory are shared
SHARED_LOCAL_MEM="6144 12288 18432"
DSAG_ENABLE=0


host $ 
host $
for SHARED_LOCAL_MEM in $SHARED_LOCAL_MEM
// Creat cgroup with memory.limit=$SHARED_LOCAL_MEM
do
    sudo ./run_lc_vm.sh $DSAG_ENABLE $SHARED_LOCAL_MEM $VM_NUM_VCPU $VM_TOTAL_MEM
    sudo ./run_be_vm1.sh $DSAG_ENABLE $SHARED_LOCAL_MEM $VM_NUM_VCPU $VM_TOTAL_MEM
    sudo ./run_be_vm2.sh $DSAG_ENABLE $SHARED_LOCAL_MEM $VM_NUM_VCPU $VM_TOTAL_MEM

    start_mapreduce ${BE_VM1}
    start_mapreduce ${BE_VM2}
done
