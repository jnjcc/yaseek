## Copyright (c) 2012, 2014 Yste.org
## All Rights Reserved.

HADOOP_BIN="/path/to/hadoop"
HADOOP_PYTHON_PATH="hdfs://path/to/python.tar.gz"

HADOOP_MEM_LIMIT=2048
HADOOP_MAP_CAPACITY=500
HADOOP_MAP_TASKS=500
HADOOP_MAP_FAIL_PERCNT=10
HADOOP_REDUCE_CAPACITY=500
HADOOP_REDUCE_TASKS=500
HADOOP_REDUCE_FAIL_PERCNT=10

## hadoop streaming job with python mapper and cat reducer, the template
hadoop_py_cat() {
    if [ $# -lt 4 ]; then
        echo "Usage: hadoop_py_cat JOBNAME INPUTDIR OUTPUTDIR MAPDIR"
        return
    fi

    JOBNAME="$1"; INDIR="$2"; OUDIR="$3"; MAPDIR="$4"
    MAPPRG=`basename ${MAPDIR}`

    $HADOOP_BIN dfs -rmr $OUDIR
    $HADOOP_BIN streaming \
        -D mapred.job.priority='NORMAL' \
        -D mapred.job.name=${JOBNAME} \
        -D stream.memory.limit=${HADOOP_MEM_LIMIT} \
        -D mapred.job.map.capacity=${HADOOP_MAP_CAPACITY} \
        -D mapred.map.over.capacity.allowed=false \
        -D mapred.job.map.tasks=${HADOOP_MAP_TASKS} \
        -D mapred.max.map.failures.percent=${HADOOP_MAP_FAIL_PERCNT} \
        -D mapred.job.reduce.capacity=${HADOOP_REDUCE_CAPACITY} \
        -D mapred.reduce.over.capacity.allowed=false \
        -D mapred.reduce.tasks=${HADOOP_REDUCE_TASKS} \
        -D mapred.max.reduce.failures.percent=${HADOOP_REDUCE_FAIL_PERCNT} \
        -D stream.map.output.field.separator='\t' \
        -D stream.num.map.output.key.fields=1 \
        -mapper "./python/bin/python ${MAPPRG}" \
        -input "${INDIR}" \
        -output "${OUDIR}" \
        -file "${MAPDIR}" \
        -cacheArchive "${HADOOP_PYTHON_PATH}#python"
}

## hadoop streaming job with python mapper and null reducer
hadoop_py_null() {
    if [ $# -lt 4 ]; then
        echo "Usage: hadoop_py_null JOBNAME INPUTDIR OUTPUTDIR MAPDIR"
        return
    fi

    JOBNAME="$1"; INDIR="$2"; OUDIR="$3"; MAPDIR="$4"
    MAPPRG=`basename ${MAPDIR}`

    $HADOOP_BIN dfs -rmr $OUDIR
    $HADOOP_BIN streaming \
        -D mapred.job.priority='NORMAL' \
        -D mapred.job.name=${JOBNAME} \
        -D stream.memory.limit=${HADOOP_MEM_LIMIT} \
        -D mapred.job.map.capacity=${HADOOP_MAP_CAPACITY} \
        -D mapred.map.over.capacity.allowed=false \
        -D mapred.job.map.tasks=${HADOOP_MAP_TASKS} \
        -D mapred.max.map.failures.percent=${HADOOP_MAP_FAIL_PERCNT} \
        -D mapred.job.reduce.capacity=${HADOOP_REDUCE_CAPACITY} \
        -D mapred.reduce.over.capacity.allowed=false \
        -D mapred.reduce.tasks=${HADOOP_REDUCE_TASKS} \
        -D mapred.max.reduce.failures.percent=${HADOOP_REDUCE_FAIL_PERCNT} \
        -D stream.map.output.field.separator='\t' \
        -D stream.num.map.output.key.fields=1 \
        -mapper "./python/bin/python ${MAPPRG}" \
        -reducer NONE \
        -input "${INDIR}" \
        -output "${OUDIR}" \
        -file "${MAPDIR}" \
        -cacheArchive "${HADOOP_PYTHON_PATH}#python"
}

HADOOP_DISTCP_SPEED=30000
HADOOP_DISTCP_MAPS=500
hadoop_copy() {
    if [ $# -lt 4 ]; then
        echo "Usage: hadoop_copy SRC_UGI DST_UGI SRC_HDFS DST_HDFS"
        return
    fi

    SUGI="$1"; DUGI="$2"; SHDFS="$3"; DHDFS="$4"

    $HADOOP_BIN distcp \
        -D distcp.map.speed.kb=${HADOOP_DISTCP_SPEED} \
        -m ${HADOOP_DISTCP_MAPS} \
        -su ${SUGI} -du ${DUGI} \
        "${SHDFS}" "${DHDFS}"
}
