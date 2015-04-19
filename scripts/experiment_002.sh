#!/usr/bin/zsh

echo "Idc in [0, 1.5] with step 0.0001"
echo -n "Running"
rm -f results/vi_002.data
for Idc_start in `seq 0 0.0001 1.5`
do
    echo -ne "\rIdc_start = $Idc_start" 1>&2;
    ./bin/1jja_c display=2 method=4 \
	Idc_start=$Idc_start \
	Omega_1=0.400000 \
	i_1=0.300000 \
	phi_start=0.000000 \
	nAv=3 \
	Omega_0=0.400000 \
	b=0.010000 \
	r=0.000000 \
	dt=0.001000 \
	t-start=40.000000 \
	o=stdout

done >> results/vi_002.data
echo -e "\rDone"
