with open("sent-pattern.txt", 'r') as sent_f:
	with open("../capture/recv-cap.txt", 'r') as recv_f:
		send = sent_f.readlines()
		recv = recv_f.readlines()
		frame_recs = {}
		for line in send:
			frameno, time = line.split()
			frame_recs[int(frameno)] = (int(time), [])
		for line in recv:
			frameno, time = line.split()
			if frame_recs.get(int(frameno)) is None:
				print "Error: frame %s recieved but not sent" %frameno
				continue
			frame_recs[int(frameno)][1].append(int(time))

		end = False
		for frame in sorted(frame_recs.keys()):
			if len(frame_recs[frame][1]) == 0:
				print "Frames %s not captured" %frame
				continue
			
			if len(frame_recs[frame][1]) > 1:
				print "    !!! Frame %d was received %d times." %(frame, len(frame_recs[frame][1]))
			print "Frame #%d: %f ms" %(frame, (min(frame_recs[frame][1]) - frame_recs[frame][0]) / 1000.0)

