# Orbit

Preparation:
1.Download all files into a folder.
2.Go to http://pygame.org/download.shtml find an appropriate py2.7 version for your platform.
3.Use IDLE to open atlas_fakerobot.py and atlas_gcs.py.

a) Test WASD direction control
1.Run fakerobot first, then run gcs. There will be a console for fakerobot, and a console and a small black window for gcs. Flat them for convenience. Focus on small black window.
2.Now you can see there are many lines rolling in two consoles, in gcs console, they indicate the status and commands inside gcs program and in fakerobot console, they indicate the data received by "robot"(see command table to decode them for correctness verification).
3.Press either W,A,S or D(multi-press has not been supported yet), you should see there are both some changes in consoles. Notice that I only use motor0 and 1 now, and the interger format is True Form now(may debug this later).

b) Test emergency connection break(only for gcs(ground control station) side now)
1.Run fakerobot first, then run gcs. There will be a console for fakerobot, and a console and a small black window for gcs. Flat them for convenience. Focus on small black window.
2.Now you can see there are many lines rolling in two consoles, in gcs console, they indicate the status and commands inside gcs program and in fakerobot console, they indicate the data received by "robot"(see command table to decode them for correctness verification).
3.Close fakerobot now, you should see that gcs print out "recover package sent". Then re-execute fakerobot, you can see gcs output recoverd.
