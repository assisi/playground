Rationale
=========

This provides a basic test of the speed that the simulator is running at, given
some artificial load of a number of bees and optionally CASUs.

Limitations
===========

Clearly this does not represent full usage, but rather, provides controlled
conditions under which a lower-bound for where overloading occurs can be
identified.

Usage
=====

Typical usage: one run

    $ assisi_playground --pub_addr tcp://*:5155 --sub_addr tcp://*:5156 &
    $ python slowdown.py -c 0 -n 30 -r 10 -s 1 -pp 5155 -sp 5156 

- `-n 30` sets 30 bees (including the focal, measured bee)
- `-c 0`  sets 0 casus
- `-s 1`  causes bees and casus to be spawned as necessary
- `-r 10` carries out 10 repeated measurements before exiting

- nonstandard port specification possible, meaning that multiple instances can
  be tested simultaneously. Set `-pp` and `-sp`, and `--hostname`, if not using
  the playground standard localhost:5556/5555.


Typical usage: convenience script for one run

    $ ./wrap_slowdown.sh 30 0

Typical usage: measure across many values of n

    $ for n in `seq 1 3 100`; do  echo $n bees; ./wrap_slowdown.sh ${n} 0 | tee -a results_0casu.log ; done
    $ cat results_0casu.log | awk -F "," 'NF>9' > data_0casu.csv


Interpreting results
====================

A graphing script is provided to generate two plots from one series of data.
It is written to be used interactively (e.g. with ipython), but if this is not 
desired, some simple edits could be made
- remove the call `plt.ion()`
- add some calls to `plt.savefig()` after each graphic.

   $ ipython
   In [1]: %run plot_results.py --datalist sample_data/example_data.list --datapath sample_data

Some example test runs are included, for comparison and to help show how to use
the scripts.



TODO
====

At this stage, the repository commits are not recorded, and annotation
regarding the machine (OS, processor, etc) is entirely manual.  





