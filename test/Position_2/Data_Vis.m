Motions = motion2

time = 50:50:size(Motions)*50;

low_pass_filt = designfilt('lowpassfir', 'FilterOrder', 10, 'CutoffFrequency',  200, 'SampleRate', 1000);

subplot(2, 3, 1);
plot(time, filter(low_pass_filt ,Motions.AccX));
title("AccX");

subplot(2, 3, 2);
plot(time, filter(low_pass_filt ,Motions.AccY));
title("AccY");

subplot(2, 3, 3);
plot(time, filter(low_pass_filt ,Motions.AccZ));
title("AccZ");

subplot(2, 3, 4);
plot(time, filter(low_pass_filt ,Motions.GyroX));
title("GyroX");

subplot(2, 3, 5);
plot(time, filter(low_pass_filt ,Motions.GyroY));
title("GyroY");

subplot(2, 3, 6);
plot(time, filter(low_pass_filt ,Motions.GyroZ));
title("GyroZ");