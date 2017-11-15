clear;
load('../data/digit.mat');
double dataset ;
for i = 1:300
    img(i) = train{1,i};
    ti = img(:);
end
