import sys

with open("scan.log", 'r') as sent_f:
    with open("../capture/scan.log", 'r') as recv_f:
        send = sent_f.readlines()
        recv = recv_f.readlines()
        frame_recs = {}
        for line in send:
            frameno, ul, rl, time = line.split()
            if int(ul) != int(rl):
                print "Error frame #%s sent has ul=%s and rl=%s" %(frameno, ul, rl) 
                sys.exit(1)
            frame_recs[int(rl)] = (int(time), [])
        for line in recv:
            frameno, ul, rl, time = line.split()
            if int(ul) != int(rl):
                print "Error frame #%s received has ul=%s and rl=%s" %(frameno, ul, rl) 
                sys.exit(1)
            if frame_recs.get(int(ul)) is None:
                print "Error: frame %s recieved but not sent" %frameno
                continue
            frame_recs[int(ul)][1].append(int(time))

        end = False
        for frame in sorted(frame_recs.keys()):
            if len(frame_recs[frame][1]) == 0:
                print "Frames %s not captured" %frame
                continue
                        
            if len(frame_recs[frame][1]) > 1:
                print "    !!! Frame %d was received %d times." %(frame, len(frame_recs[frame][1]))
            print "Frame #%d: %f ms" %(frame, (min(frame_recs[frame][1]) - frame_recs[frame][0]) / 1000.0)

