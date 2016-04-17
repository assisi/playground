#!/bin/sh

# start playground, in bg
# capture PID
# run slowdown (which spawns, does measurement n times), in fg
# kill PID(playground)
# done

# typical wrap-wrap case
#for in `seq 1 3 100`; do  echo $i bees; python slowdown.py -r 10 -n $i -s 1 | tee -a results_nth_20casu_nogui.log ; done
# cat results_nth_0casu_ssd.log | awk -F "," 'NF>9'
n_bees=${1:-1}
n_casu=${2:-10}
n_rpts=10
spawn=1

# playground needs the * in address, but clients need a specific host name
pp=5155
sp=5156
pub_addr=tcp://*:${pp}
sub_addr=tcp://*:${sp}

#assisi_playground --nogui &
echo assisi_playground --pub_addr ${pub_addr} --sub_addr ${sub_addr}
PG_PID=$!


echo "PID of playground: ${PG_PID}"
echo "...exec this cmd: python slowdown.py -c ${n_casu} -n ${n_bees} -r ${n_rpts} -s ${spawn}"

echo python slowdown.py -c ${n_casu} -n ${n_bees} -r ${n_rpts} -s ${spawn} -pp ${pp} -sp ${sp}

exit 1

echo "killing PG ${PG_PID}"

kill -2 ${PG_PID}
kill -15 ${PG_PID}

