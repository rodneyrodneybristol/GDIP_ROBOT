% Robot coordinate system calculations
% using E12 Resisitors meaning 12 resistors, 12 boxes for specific
% resistors plus one for extra/unrecognizable.
% Dimensions of individual box = 54 * 79 mm 
% Brief requirement: Arm must extend to 50cm 
% Taking centre of robot to be origin point
% all measurement in mm
%
% a and b needs to be under 165 to stay within working envelope
%% Assiging Variables
% a =100 ;
% b = 50;
% alpha = a+5;
% beta = b + 176;
alpha = -40
beta = 477.5
%% Generating coordinates for box/grid
% Box_BLHS = [a , b] % box bottom left hand side
% Box_BRHS = [a+240, b]
% Box_TLHS = [a , b+340]
% Box_TRHS = [a+240, b+340]

%% Generating coordinates for individual boxes/trays
%taking alpha and beta as the reference point
% itl = individual tray length
% itw = individual tray width
itl = 80;
itw = 55;
% c  = clearance distance = spacing between boxes
c = 5;
% [a,b ;b,a] this is the format of code needed to generate correct data
% T1 = [alpha+c,beta+176 , alpha+5+itl,beta+176 ; alpha+c,beta+176-itw , alpha+5+itl,beta+176-itw] % [top LLHS, top RHS ; bott LHS , bott RHS]
% T2 = [alpha+c+c+ibl,beta+176 , alpha+c+c+ibl+ibl,beta+176 ; alpha+c+c+ibl,beta+176-itw , alpha+c+c+ibl+ibl, beta+176-itw  ]
%tediuous method, resulting in long code, easier to make a mistake coding

% using the Top LHS for reference point (alpha,beta)
ref = [alpha,beta]

T1 = [ref,ref+[0,itl] ; ref + [itw,0] , ref+[itw,itl]]
T2 = [ref+[0,itl+c], ref + [0,itl+itl+c] ; ref+[itw,itl+c] , ref+[itw,itl+itl+c]]
T3 = [ref+[0,itl+itl+c+c], ref+[0,itl+itl+itl+c+c]; ref+[itw,itl+itl+c+c], ref+[itw,itl+itl+itl+c+c]]
% T4 = [ref+[itl+c+itl+c+itl+c,0],ref+[itl+c+itl+c+itl+c+itl,0] ; ref+[itl+c+itl+c+itl+c,-itw], ref+[itl+c+itl+c+itl+c+itl,itw]]
T4 = [ref+[itw+c,0], ref+[itw+c,itl]; ref+[itw+c+itw,0] , ref+[itw+c+itw,itl]]
T5 = [ref+[itw+c,itl+c], ref+[itw+c,itl+itl+c] ; ref+[itw+c+itw,itl+c], ref+[itw+c+itw,itl+itl+c]]
T6 = [ref+[itw+c,itl+itl+c+c],ref+[itw+c,itl+itl+itl+c+c] ; ref+[itw+c+itw,itl+itl+c+c] , ref+[itw+c+itw,itl+itl+itl+c+c]]
% T8 = [ref+[itl+c+itl+c+itl+c,-itw-c], ref+[itl+c+itl+c+itl+c+itl,-itw-c]; ref+[itl+c+itl+c+itl+c,-itw-c-itw],ref+[itl+c+itl+c+itl+c+itl,-itw-c-itw]]
T7 = [ref+[itw+c+itw+c,0],ref+[itw+c+itw+c,itl]; ref+[itw+c+itw+c+itw,0], ref+[itw+c+itw+c+itw,itl]]
T8 = [ref+[itw+c+itw+c,itl+c], ref+[itw+c+itw+c,itl+itl+c]; ref+[itw+c+itw+c+itw,itl+c], ref+[itw+c+itw+c+itw,itl+itl+c]]
T9 = [ref+[itw+c+itw+c,itl+itl+c+c,], ref+[itw+c+itw+c,itl+itl+itl+c+c]; ref+[itw+c+itw+c+itw,itl+itl+c+c,], ref+[itw+c+itw+c+itw,itl+itl+itl+c+c]]
T_EX_1 = [ref+[itw+c+itw+c,itl+c+itl+c+itl+c], ref+[itw+c+itw+c,itl+c+itl+c+itl+c+itl]; ref+[itw+c+itw+c+itw,itl+c+itl+c+itl+c] , ref+[itw+c+itw+c+itw,itl+c+itl+c+itl+c+itl]]
%one extra tray needed for misc resistors,will be named T_EX
% change of design means this is now redundant :T_EX = [ref+[itl+c+(itl/2),c+itw], ref+[itl+c+(itl/2)+itl,c+itw] ; ref+[itl+c+(itl/2),c] , ref+[itl+c+(itl/2)+itl,c]]
T10 = [ref+[itw+c+itw+c+itw+c,0],ref+[itw+c+itw+c+itw+c,itl]; ref+[itw+c+itw+c+itw+c+itw,0], ref+[itw+c+itw+c+itw+c+itw,itl]]
T11 = [ref+[itw+c+itw+c+itw+c,itl+c], ref+[itw+c+itw+c+itw+c,itl+c+itl]; ref+[itw+c+itw+c+itw+c+itw,itl+c], ref+[itw+c+itw+c+itw+c+itw,itl+c+itl]]
T12 = [ref+[itw+c+itw+c+itw+c,itl+c+itl+c], ref+[itw+c+itw+c+itw+c,itl+itl+itl+c+c]; ref+[itw+c+itw+c+itw+c+itw,itl+itl+c+c,], ref+[itw+c+itw+c+itw+c+itw,itl+itl+itl+c+c]]
T_EX_2 = [ref+[itw+c+itw+c+itw+c,itl+c+itl+c+itl+c], ref+[itw+c+itw+c+itw+c,itl+c+itl+c+itl+c+itl]; ref+[itw+c+itw+c+itw+c+itw,itl+c+itl+c+itl+c], ref+[itw+c+itw+c+itw+c+itw,itl+c+itl+c+itl+c+itl]]
%% finding the midpoint of each tray for forward kinematics
%for i<13;
 %   i = 1
  %  M_T(i) = [[((T(i))(1) + (T(i))(5))/2],[((T(i))(3) + (T(i))(7))/2] ]
   % i = i+1
%tried using for loops but couldn't get it to work

% M_T = midpoint of T()
M_T1 = [(T1(1)+T1(2))/2,(T1(3)+T1(7))/2]
M_T2 = [(T2(1)+T2(2))/2,(T2(3)+T2(7))/2]
M_T3 = [(T3(1)+T3(2))/2,(T3(3)+T3(7))/2]
M_T4 = [(T4(1)+T4(6))/2,(T4(3)+T4(7))/2]
M_T5 = [(T5(1)+T5(2))/2,(T5(3)+T5(7))/2]
M_T6 = [(T6(1)+T6(2))/2,(T6(3)+T6(7))/2]
M_T7 = [(T7(1)+T7(2))/2,(T7(3)+T7(7))/2]
M_T8 = [(T8(1)+T8(2))/2,(T8(3)+T8(7))/2]
M_T9 = [(T9(1)+T9(2))/2,(T9(3)+T9(7))/2]
M_T10 = [(T10(1)+T10(2))/2,(T10(3)+T10(7))/2]
M_T11 = [(T11(1)+T11(2))/2,(T11(3)+T11(7))/2]
M_T12 = [(T12(1)+T12(2))/2,(T12(3)+T12(7))/2]
M_T_EX_1 = [(T_EX_1(1)+T_EX_1(2))/2,(T_EX_1(3)+T_EX_1(7))/2]
M_T_EX_2 = [(T_EX_2(1)+T_EX_2(2))/2,(T_EX_2(3)+T_EX_2(7))/2]
%% finding the length between base and midpoint of the tray

L1 = sqrt(M_T1(1)^2 + M_T1(2)^2)
L2 = sqrt(M_T2(1)^2 + M_T2(2)^2)
L3 = sqrt(M_T3(1)^2 + M_T3(2)^2)
L4 = sqrt(M_T4(1)^2 + M_T4(2)^2)
L5 = sqrt(M_T5(1)^2 + M_T5(2)^2)
L6 = sqrt(M_T6(1)^2 + M_T6(2)^2)
L7 = sqrt(M_T7(1)^2 + M_T7(2)^2)
L8 = sqrt(M_T8(1)^2 + M_T8(2)^2)
L9 = sqrt(M_T9(1)^2 + M_T9(2)^2)
L10 = sqrt(M_T10(1)^2 + M_T10(2)^2)
L11 = sqrt(M_T11(1)^2 + M_T11(2)^2)
L12 = sqrt(M_T12(1)^2 + M_T12(2)^2)
L_EX_1 = sqrt(M_T_EX_1(1)^2 + M_T_EX_1(2)^2)
L_EX_2 = sqrt(M_T_EX_2(1)^2 + M_T_EX_2(2)^2)
T_L = [L1,L2,L3,L4;L5,L6,L7,L8;L9,L10,L11,L12] % putting all of them into a table


%% Finding the angle between base and midpoint of tray
% for angle calculations, best to have the modulus of coordinates for
% T1,2,3
theta_1_1 = atand(M_T1(2)/abs(M_T1(1))) % in degrees
theta_1_2 = atand(M_T2(2)/abs(M_T2(1)))
theta_1_3 = atand(M_T3(2)/abs(M_T3(1)))
theta_1_4 = atand(M_T4(2)/M_T4(1))
theta_1_5 = atand(M_T5(2)/M_T5(1))
theta_1_6 = atand(M_T6(2)/M_T6(1))
theta_1_7 = atand(M_T7(2)/M_T7(1))
theta_1_8 = atand(M_T8(2)/M_T8(1))
theta_1_9 = atand(M_T9(2)/M_T9(1))
theta_1_10 = atand(M_T10(2)/M_T10(1))
theta_1_11 = atand(M_T11(2)/M_T11(1))
theta_1_12 = atand(M_T12(2)/M_T12(1))
theta_1_EX_1 = atand(M_T_EX_1(2)/M_T_EX_1(1))
theta_1_EX_2 = atand(M_T_EX_2(2)/M_T_EX_2(1))

T_theta = [theta_1_1,theta_1_2,theta_1_3,theta_1_4;theta_1_5,theta_1_6,theta_1_7,theta_1_8;theta_1_9,theta_1_10,theta_1_11,theta_1_12] % all in one gtable 


%% robot arm lengths

d1 = 146.05; % shoulder to elbow length
d2 = 222.26; %elbow to wrist length , will need to be extended tho
d3 = 85.725; % wrist to gripper length
% assuming the resistors will be at the same elevation as the base of robot
% will restrict the movement of wrist joint by keeping angle constant at 90
% % can calculate the distance end effector can reach without moving shoulder
% joint, calculated using Pythagoras to be 313.84 anything greater than that will require
% shoulder servo rotation
% finding h

h = sqrt(d2^2-(d1-d3)^2)
theta_3 = asind(h/d2) % if the distance to reach is greater than h, theta 2 will be set to this value
%% theta 2 calcs
% if L is less than h, theta 2 will be 0, otherwise theta 3 will be
% constant and theta 2 will be determined
% currently L is less than h for tray 7 and 10
x_1 = L1-h
x_2 = L2-h
x_3 = L3-h
x_4 = L4-h
x_5 = L5-h
x_6 = L6-h
x_7 = L7-h
x_8 = L8-h
x_9 = L9-h
x_10 = L10-h
x_11 = L11-h
x_12 = L12-h
x_EX_1 = L_EX_1 - h
x_EX_2 = L_EX_2 - h

theta_2_1 = atand(x_1/d1)
theta_2_2 = atand(x_2/d1)
theta_2_3 = atand(x_3/d1)
theta_2_4 = atand(x_4/d1)
theta_2_5 = atand(x_5/d1)
theta_2_6 = atand(x_6/d1)
theta_2_7 = atand(x_7/d1)
theta_2_8 = atand(x_8/d1)
theta_2_9 = atand(x_9/d1) 
theta_2_10 = atand(x_10/d1)
theta_2_11= atand(x_11/d1)
theta_2_12 = atand(x_12/d1)
theta_2_EX_1 = atand(x_EX_1/d1)
theta_2_EX_2 = atand(x_EX_2/d1)


theta_3_1 = theta_3
theta_3_2 = theta_3
theta_3_3 = theta_3
theta_3_4 = theta_3
theta_3_5 = theta_3
theta_3_6 = theta_3
theta_3_7 = theta_3
theta_3_8 = theta_3
theta_3_9 = theta_3
theta_3_10 = theta_3
theta_3_11= theta_3
theta_3_12 = theta_3
theta_3_EX_1 = theta_3
theta_3_EX_2 = theta_3

% theta 4 is constant
theta_4 = 90
