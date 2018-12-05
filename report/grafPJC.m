close all;
clc;
size =[4 8 16 32 64 128 256 512 1024];
single =[0 0 0 3 20 156 1276.6 10782 84849];
parallel = [0.5 0.3 0 0.6 4.4 37.4 292.5 2354.5 18486.7];

figure;
hold on;
plot(size, single,'LineWidth',4)
plot(size, parallel,'LineWidth',4)
grid

title('Graf porovnání rychlosti jedno a vícevláknového algoritmu','fontsize',40)
x=xlabel('velikost matice')
y=ylabel('t [ms]')
x.FontSize = 30;
y.FontSize = 30;
lgd=legend('1 vlákno','8 vláken')
lgd.FontSize=30;