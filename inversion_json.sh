#!/bin/sh

export LANG=en_GB.UTF-8

region=europe
type="TEXT%3ALIST"
yyyy=${1}
mm=${2}
dd=${3}
hh=${4}
kennz=10410

rm sounding*

wget http://weather.uwyo.edu/cgi-bin/sounding\?region\=${region}\&TYPE\=${type}\&YEAR\=${yyyy}\&MONTH\=${mm}\&FROM\=${dd}${hh}\&TO\=${dd}${hh}\&STNM\=${kennz}

mv sounding\?region\=${region}\&TYPE\=${type}\&YEAR\=${yyyy}\&MONTH\=${mm}\&FROM\=${dd}${hh}\&TO\=${dd}${hh}\&STNM\=${kennz} out.txt


awk '{
      if ( ( NF == 11 ) && ( $1 != "PRES" ) && ( $1 != "hPa" ) && ( $1 != "Questions" ) )  print$1,$2,$3,$7,$8;
     }' out.txt > dat1

awk 'BEGIN{i=0;}
     {
      p[i]=$1;
      z[i]=$2;
      t[i]=$3;
      dd[i]=$4;
      ff[i]=$5;
      i++;
     }END{
          ib=0;
          n=0;
          printf("{\nInversion_layers_below_700_hPa:\n[\n");
          for (j=0;j<i-1;j++)
          {
#bottom of inversion
           if (  ( t[j] < t[j+1] ) && ( jb == 0 ) ) {jb=j;}
#top of inversion
           if ( ( jb > 0 ) && ( t[j] > t[j+1] ) )
           {
            n++;
            lt=z[j]-z[jb];
            dt=t[j]-t[jb];
            tg=100.0*dt/lt;
            if ( ( n > 1 ) && ( p[jb] > 700 ) )  printf(",\n");
            if ( p[jb] > 700 ) printf(" {bottom_inversion_hPa: %6.1f, top_inversion_hPa: %6.1f, bottom_inversion_metric: %6.1f, top_inversion_metric: %6.1f, layer_thickness_metric: %6.1f, temperature_increase: %6.1f, temp_gradient_celsius_100meter: %6.1f}",p[jb],p[j],z[jb],z[j],lt,dt,tg);
            jb=0;
           }
          }
          printf("\n],\n");
          printf("temperature_gradients_surface_to_x_hPa:\n[\n");
          n=0;
          for (j=0;j<i;j++)
          {
           if ( ( p[j] == 925 ) || ( p[j] == 850 ) || ( p[j] == 700 ) )
           {
            n++;
            tg=100.0*(t[j]-t[0])/(z[j]-z[0]);
            if ( n > 1 ) printf(",\n");
            printf(" { pressure: %6.1f, avg_temp_gradient_cels_100meter: %6.1f}",p[j],tg);
           }
          }
          printf("\n],\n");
          printf("mean_wind_conditions:\n");
          n=0;
#surface - 925
          usum=0.0;
          vsum=0.0;
          for (j=0;j<i;j++)
          {
           if ( p[j] == 925 )
           {
            uavg=usum/n;
            vavg=vsum/n;
            ffavg=sqrt(uavg*uavg+vavg*vavg);
            if ( ffavg > 0 ) {dddavg=acos(-vavg/ffavg)*180/3.14159265;}
            else {dddavg=-99.9;}
            if ( ( uavg >= 0 ) && ( dddavg > -99 ) )  dddavg=360-dddavg;
            printf("{\n 925_over_ground_pressure: [ mean_wind_direction_in_degree: %6.1f, mean_wind_speed_knots: %6.1f],\n",dddavg,ffavg);
           }
           else if ( p[j] > 925 )
           {
            u=-ff[j]*sin(3.1416*dd[j]/180);
            v=-ff[j]*cos(3.1416*dd[j]/180);
            usum+=u;
            vsum+=v;
            n++;
           }
          }
#925-700
          usum=0.0;
          vsum=0.0;
          for (j=0;j<i;j++)
          {
           if ( p[j] == 700 )
           {
            uavg=usum/n;
            vavg=vsum/n;
            ffavg=sqrt(uavg*uavg+vavg*vavg);
            if ( ffavg > 0 ) {dddavg=acos(-vavg/ffavg)*180/3.14159265;}
            else {dddavg=-99.9;}
            if ( ( uavg >= 0 ) && ( dddavg > -99 ) )  dddavg=360-dddavg;
            printf(" between_925_and_700_pressure: [ mean_wind_direction_in_degree: %6.1f, mean_wind_speed_knots: %6.1f]\n},\n",dddavg,ffavg);
           }
           else if ( p[j] > 700 )
           {
            u=-ff[j]*sin(3.1416*dd[j]/180);
            v=-ff[j]*cos(3.1416*dd[j]/180);
            usum+=u;
            vsum+=v;
            n++;
           }
          }
         } 
         function abs(x)  { return x<0?x*-1:x;}
         function acos(x) { return atan2(sqrt(abs(1-x*x)), x) }' dat1 > dat2

grep "Convective Inhibition" out.txt | sed 's/Convective Inhibition/convective_inhibition/g' | awk '{printf("%s %s,\n",$1,$2);}' >> dat2
grep "Convective Available" out.txt | sed 's/Convective Available Potential Energy/convective_available_potential_energy/g' | awk '{printf("%s %s\n}\n",$1,$2);}' >> dat2

exit
# Temperature increase (dt) => ok
# Layer thickness (lt) => ok
# Temperature gradient (dt/100 m) => ok
# mean wind speed and direction surface to 925 hPa and 925 to 700 hPa
# Temperature gradients surface to 925 hPa, surface to 850 hPa, surface to 700 hPa


