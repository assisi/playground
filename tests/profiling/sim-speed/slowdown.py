from assisipy import bee
import time
import argparse, sys

from assisipy import sim
from math import pi, sin, cos
import random
from numpy import linspace, deg2rad, zeros



#{{{ define behaviours

def stop(abee):
    abee.set_color(r=0.75, g=0.75, b=0.75)
    abee.set_vel(0, 0)

def fwd(abee, vel=2.75):
    abee.set_color(r=1, g=1, b=0)
    abee.set_vel(vel, vel)


def move_for_time(abee, dt=1.0, vel=2.0):
    fwd(abee, vel=vel)
    time.sleep(dt)
    stop(abee)

def meas_after_accel(abee, dt=1.0, vel=2.0, lag=0.25):
    '''
    attempt a more accurate measurement by ignoring some period
    where there may be accelaration occurring
    '''
    fwd(abee, vel=vel)
    #print "[I] vel ref rq {}, current is".format(vel), abee.get_vel_ref()
    time.sleep(lag)
    p1 = abee.get_true_pose()
    time.sleep(dt)
    p2 = abee.get_true_pose()
    stop(abee)
    return p1, p2

#}}}

#{{{ measurement functions
def meas(abee, dt=1.0, vel=2.0):
    stop(abee)
    p1 = abee.get_true_pose()
    move_for_time(abee, dt=dt, vel=vel)
    p2 = abee.get_true_pose()
    return p1, p2

def dst(p1, p2):
    return ((p1[0] - p2[0])**2 + (p1[1] - p2[1])**2)**0.5


def err_meas(abee, dt=1.0, vel=2.0):
    '''given target speed and time moved for, compare with actual and print error'''
    p1, p2 = meas(mybee, dt=dt, vel=vel)
    d = dst(p1, p2)
    est_d = vel * dt
    err_d = float(d - est_d) / est_d
    print "{:.2f} {:.2f} -> {:.2f} {:.2f}".format(p1[0], p1[1], p2[0], p2[1])
    print "target : {:.2f}, actual {:.2f}, err (abs|rel): {:.2f}|{:.2f}%".format(est_d, d, abs(d - est_d), err_d*100.)
    return err_d

def err_meas2(abee, dt=1.0, vel=2.0, lag=0.25, verb=True):
    '''given target speed and time moved for, compare with actual and print error'''
    p1, p2 = meas_after_accel(abee, dt=dt, vel=vel, lag=lag)
    d = dst(p1, p2)
    est_d = vel * dt
    err_d = float(d - est_d) / est_d
    if verb:
        print "{:.2f} {:.2f} -> {:.2f} {:.2f}".format(
                p1[0], p1[1], p2[0], p2[1])
        print "target : {:.2f}, actual {:.2f}, err (abs|rel): {:.2f}|{:.2f}%".format(
                est_d, d, abs(d - est_d), err_d*100.)
    return d, est_d, err_d

#}}}

#{{{ spawning
def spawn_extras(rng, FACE_SOUTH=True):
    for i in rng:
        name = 'bee-{:03d}'.format(i)
        yaw = 90 + random.uniform(-10,10)
        if FACE_SOUTH:
            yaw += 180
        yaw *= pi / 180
        x = random.uniform(-10, +10)
        y = -2
        simctrl.spawn('Bee', name, (x, y, yaw))


def spawn_radial_casus(n_casus, rad=19):
    # new geometry
    # circles -- the sensors turned on will have to be a bit cleverer;
    #   maybe compute based on a get_true_pos() at init?


    blow_theta = linspace(0, 360, endpoint=False, num=n_casus)
    for idx, theta in enumerate(blow_theta):
        th_rad = deg2rad(180+90-theta)
        name = 'casu-b{:02d}'.format(idx)
        tx = rad * sin(deg2rad(theta))
        ty = rad * cos(deg2rad(theta))
        s = "Casu, {}, {:.4f}, {:.4f}, {:.4f}".format(name, tx, ty, th_rad)
        print s
        #f.write(s + "\n")
        simctrl.spawn('Casu', name, (tx, ty, th_rad))

#}}}

#{{{ idea/notes
'''

The underlying question is:
- what is the scalability of the simulator?
- what are the bottlenecks, what dominates the scaling?
- (how can we identify when something is not right?)

The method here to measure it is
- assume that the speed over time is implemted accurately
- move one target bee for a specific time and velocity
- measure error between expected movement and actual movement

vary
a) the number of bees spawned
b) whether the moving bee is in range of the others
   (just have them face N or S, +- sig)
c) number of CASUs spawned
d) fraction of bees spawned with handlers attached
e) fraction of CASUs spawned with handlers attached

f) if possible, a-e in "headless" mode


- possible confounding effects
   - accelaration period (if I start from vel=0, will I underestimate dist?)
   - measurement time (if I say "get me pos now, when was it last computed?)


'''

'''
specific procedure

- initially, lets look at relevant distances to travel to avoid
  measuring accel period
  and to avoid measurement error (at least check what there is)

- then we have the setup: how far to travel
  - we can do tests progressively introducing new bees
  - we can't go backwards (well there is a "kill" but lets start from
    clean slate each time)


- generate data to draw a graph with
   - num bees (x)
   - distance travelled in k seconds (y)
   - relative error in time (y2)

'''
#}}}



if __name__ == '__main__':
    # process input
    parser = argparse.ArgumentParser()
    parser.add_argument('--logfile', type=str, default=None,)
    parser.add_argument('-n', '--n_bees', type=int, default=1)
    parser.add_argument('-c', '--n_casu', type=int, default=0)
    parser.add_argument('-r', '--rpts', type=int, default=5)
    parser.add_argument('-s', '--spawn', type=int, default=0)
    parser.add_argument('--hostname', type=str, default="localhost",)
    parser.add_argument('-pp', '--pub-port', type=str, default="5155",
                        help="publish port of the SIMULATOR")
    parser.add_argument('-sp', '--sub-port', type=str, default="5156",
                        help="subscribe port of the SIMULATOR")

    args = parser.parse_args()
    # does the main bee trigger sensors of the distractor bees? SOUTH=>no.
    FACE_SOUTH = False

    # the server publishes on X, reads on Y means that clients here should
    # read X and publish Y. So they are reversed here intentionally
    cli_pa = "tcp://{}:{}".format(args.hostname, args.sub_port)
    cli_sa = "tcp://{}:{}".format(args.hostname, args.pub_port)

    print "#[I] Attaching sim client on pub_addr {} sub_addr {}".format(cli_pa, cli_sa)

    simctrl = sim.Control(pub_addr=cli_pa, sub_addr=cli_sa) # this is a client!

    yaw = 0
    x, y = -10, 0
    ic0 = (x,y,yaw)
    # 1. spawn;
    if args.spawn:
        if args.n_casu > 0:
            spawn_radial_casus(args.n_casu, rad=25)

        # first bee moves horizontally across arena
        name = 'bee-{:03d}'.format(0)
        simctrl.spawn('Bee', name, ic0)

        # all other bees face either N or S +sig
        spawn_extras(xrange(1, args.n_bees), FACE_SOUTH=FACE_SOUTH)


    # 2. now run behaviour of the special one.
    print "[I] attempting to connect to {}...".format('bee-000')
    mybee = bee.Bee(name='bee-000', pub_addr=cli_pa, sub_addr=cli_sa)
    print mybee.get_true_pose()


    simctrl.teleport(mybee._Bee__name, ic0)
    print "post teleport:", mybee.get_true_pose()

    # make some measurements
    t = 4.0; lag=0.5; vel = 1.5
    D = zeros((args.rpts, 3))
    for r in xrange(args.rpts):
        simctrl.teleport(mybee._Bee__name, ic0)
        #print "r {} first teleport:".format(r), mybee.get_true_pose()
        time.sleep(0.25)
        #err_d = float(d - est_d) / est_d
        act_d, est_d, err = err_meas2(mybee, dt=t, vel=vel, lag=lag, verb=False)
        time.sleep(0.25)
        #print "r {} end of run:".format(r), mybee.get_true_pose()
        D[r] = act_d, est_d, err
        sys.stdout.write(".")
        sys.stdout.flush()

    sys.stdout.write(">>Done. \n")
    print D
    # what is the mean/sd in error (abs) | rel
    #
    M = D[:,0] # measurement
    E = D[:,1] # expecattion
    a_err = E - M
    r_err = (E - M) / E

    print "#Absolute error: {:.3f} ({:.3f})".format(a_err.mean(), a_err.std())
    print "#Relative error: {:.3f} ({:.3f}) [{:.3f}%]".format(r_err.mean(), r_err.std(), 100. * r_err.mean())

    res = [args.n_bees, int(FACE_SOUTH), t, lag, vel, args.rpts, a_err.mean(), a_err.std(), r_err.mean(), r_err.std()]

    s = ",".join("{}".format(x) for x in res)
    print s




