load ttiming.txt
load mtiming.txt
load atiming.txt
% 3rd column contains the timing of 5 functions at 5 sizes repeated 6 times
T=reshape(ttiming(:,3),[5,5,6]);
M=reshape(mtiming(:,3),[5,5,6]);
A=reshape(atiming(:,3),[5,5,6]);
% 2nd column contains the problem sizes
n = unique(ttiming(:,2))
Ta=mean(T,3);
Ma=mean(M,3);
Aa=mean(A,3);

n1=@(n)n(:)';
n2=@(n)n(:)'.^2;
n3=@(n)n(:)'.^3;

figure(1)
set(1,'name','Function #1 = insert')
ix = 1;
T1=mean(reshape(ttiming(ttiming(:,1)==ix,3),6,[]),1);
M1=mean(reshape(mtiming(mtiming(:,1)==ix,3),6,[]),1);
A1=mean(reshape(atiming(atiming(:,1)==ix,3),6,[]),1);
g=n1;
subplot(2,1,1)
plot(n,T1./g(n),'x-',n,M1./g(n),'o-',n,A1./g(n),'*-')
legend('T1/n','M1/n','A1/n')
set(gca,'ylim',[0,inf],'xtick',n)
title(gca,'Some valid title')
subplot(2,1,2)
plot(n,T1./g(n),'x-',n,M1./g(n),'o-')
legend('T1/n','M1/n')
set(gca,'ylim',[0,inf],'xtick',n)
title(gca,'Some valid title')

figure(2)
set(2,'name','Function #2 = remove')
ix = 2;
T1=mean(reshape(ttiming(ttiming(:,1)==ix,3),6,[]),1);
M1=mean(reshape(mtiming(mtiming(:,1)==ix,3),6,[]),1);
A1=mean(reshape(atiming(atiming(:,1)==ix,3),6,[]),1);
g=n2;
plot(n,T1./g(n),'x-',n,M1./g(n),'o-',n,A1./g(n),'*-')
legend('T1/n^2','M1/n^2','A1/n^2')
set(gca,'ylim',[0,inf],'xtick',n)
title(gca,'Some valid title')

figure(3)
set(3,'name','Function #3..#5 = various lookup')
titles={'Failed lookup', 'Random lookup', 'Skewed lookup'};
  
for i = 1:3
    ix = 2+i;
    T1=mean(reshape(ttiming(ttiming(:,1)==ix,3),6,[]),1);
    M1=mean(reshape(mtiming(mtiming(:,1)==ix,3),6,[]),1);
    A1=mean(reshape(atiming(atiming(:,1)==ix,3),6,[]),1);
    g=n2;
    subplot(1,3,i)
    plot(n,T1./g(n),'x-',n,M1./g(n),'o-',n,A1./g(n),'*-')
    set(gca,'ylim',[0,2e-5],'xtick',n)
    title(gca,titles{i})
end
    

print -depsc -f1 /tmp/fig1.eps
print -depsc -f2 /tmp/fig2.eps
print -depsc -f3 /tmp/fig3.eps

disp('You now have three files fig1.eps, fig2.eps, fig3.eps in the /tmp folder')
disp('Under linux, do the following commands to convert them to pdf:')
disp(' ')
disp('cd /tmp')
disp('epspdf fig1.eps')
disp('epspdf fig2.eps')
disp('epspdf fig3.eps')
disp(' ')
disp('The copy the pdf files to your latex folder or insert into overleaf.')
    
% Then run the following commands in linux:
% cd /tmp
% epspdf fig1.eps
% epspdf fig2.eps
% epspdf fig3.eps
% and copy the fig1.pdf, etc., files to your latex folder.
