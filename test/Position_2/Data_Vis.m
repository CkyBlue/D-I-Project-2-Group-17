import testdata;

time = 50:50:size(testdata)*50;

figure (1);
plot(time, testdata(1, :))