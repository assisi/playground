import numpy as np
import matplotlib.pyplot as plt
import os
import argparse, yaml

class Dataset(object):
    def __init__(self, info, path=''):
        fname, label, ncasus = info # split up info list into useful vars
        fp  = os.path.join(path, fname)
        self.d = np.genfromtxt(fp, delimiter=',')
        self.fn = fname
        self.label = label
        self.ncasus = ncasus

def compute_simulation_cost(d):
    '''
    We measured the slowdown incurred from heavier computational loads;
    from this info we can also estimate how much time passes for each simulated
    second.
    This function returns a vector of real seconds elapsed per simulated second

    '''
    #--- process data ---#
    # col 4 is the velocity
    # col 2 is time run for
    # so E is col2 * col 4
    # and M is not shown directly, but col 6 is a_err = E-M;
    # thus M = E - col6
    #
    # we expectdd the following
    t_exp = d[:, 2]
    v_exp = d[:, 4]
    d_exp = t_exp * v_exp

    # we measured the following
    d_meas = d_exp - d[:,6]
    t_sim = d_meas / v_exp

    # and so the #sec (real) per sec (sim) is
    real_per_sim = t_exp / t_sim

    return real_per_sim



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--datalist', type=str, required=True,
                        help="a file containing names & labels of test data files")
    parser.add_argument('--datapath', type=str, default='.',
                        help="path containing the test data files")
    parser.add_argument('-c', '--casus-as-agents', action='store_true',
                        help="treat CASUs as bees? if true, x axis combines bees and CASUs")
    args = parser.parse_args()

    # read info about results
    #    expects the results config file to be a list of triples :
    #    ['filename1', 'label for set 1', #casus],
    #    ['filename2', 'label for set 2', #casus],
    with open(args.datalist) as f:
        results_info = yaml.safe_load(f)

    # read in results dataset
    DD = {}
    for row in results_info:
        o = Dataset(row, path=args.datapath)
        DD[o.fn] = o


    # === plot #1: consider the relative error === #
    plt.ion()
    plt.figure(1); plt.clf()

    clrs = 'brgmcky'
    for i, (fn, ds) in enumerate(DD.items()):
    #for i, (fn, lbl) in enumerate(listings):
        d = ds.d
        # set up info for this series
        n_c = 0
        if args.casus_as_agents: n_c = ds.ncasus
        lbl = ds.label
        if ds.ncasus > 0: lbl = "{}, {} casus".format(ds.label, ds.ncasus)
        clr = clrs[i % len(clrs)]

        # plot, and also show the variability
        plt.plot(d[:,0]+n_c, d[:,8], label=lbl, color=clr)
        yerr = d[:,9] # should really be SEM but this is unobservable
        plt.fill_between(n_c+d[:,0], d[:,8] - yerr,  d[:,8] + yerr, color=clr, alpha=0.35)

    if args.casus_as_agents:
        plt.xlabel('number of agents (bee+casu)')
    else:
        plt.xlabel('number of bees')
    plt.ylabel('relative error (slowdown)')
    plt.legend(loc='upper left')
    plt.grid('on')


    # === plot 2: show the cost -- seconds (realtime) per second (simtime) === #
    plt.figure(2); plt.clf()

    for i, (fn, ds) in enumerate(DD.items()):
        d = ds.d
        # perform the calculations
        real_per_sim = compute_simulation_cost(d)

        # set up info for this series
        n_c = 0
        if args.casus_as_agents: n_c = ds.ncasus
        lbl = ds.label
        if ds.ncasus > 0: lbl = "{}, {} casus".format(ds.label, ds.ncasus)
        clr = clrs[i % len(clrs)]

        #--- now the plot ---#
        plt.plot(n_c + d[:, 0], real_per_sim, label=lbl, color=clr)

    if args.casus_as_agents:
        plt.xlabel('number of agents (bee+casu)')
    else:
        plt.xlabel('number of bees')

    plt.ylabel('seconds used per simulated second')
    plt.legend(loc='upper left')
    plt.grid('on')


