#!/usr/bin/perl -w
use strict;
my $next_packet_time = 0;
my $next_rate_decrease = 0;
for (my $i = 0; $i < 10000; $i++) {
  print int( $next_packet_time ), "\n";
  print int( $next_packet_time ), "\n";
  $next_packet_time++;
}
# downward ramp
my $packets_per_millisecond = 2;
while ( $packets_per_millisecond > 0.01 ) {
  print int( $next_packet_time ), "\n";
  $next_packet_time += 1.0 / $packets_per_millisecond;
  while ( $next_packet_time - 10000 >= $next_rate_decrease ) {
    $packets_per_millisecond -= 0.0001;
    $next_rate_decrease += 1; # millisecond
  }
}
# upward ramp
while ( $packets_per_millisecond < 2 ) {
  print int( $next_packet_time ), "\n";
  $next_packet_time += 1.0 / $packets_per_millisecond;
  while ( $next_packet_time - 10000 >= $next_rate_decrease ) {
    $packets_per_millisecond += 0.0001;
    $next_rate_decrease += 1; # millisecond
  }
}
