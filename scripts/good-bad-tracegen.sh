#!/usr/bin/perl -w

use strict;

my $timestamp = 0.0;

sub Mbps_to_ppms ($) {
  my ( $Mbps ) = @_;
  my $packets_per_ms = 1e6 * $Mbps / (1500 * 8 * 1e3);
  print STDERR "megabits per second of $Mbps => packets per ms of $packets_per_ms\n";
  return $packets_per_ms;
}

my $good_Mbps = 2;
my $good_packets_per_millisecond = Mbps_to_ppms $good_Mbps;

while ( $timestamp <= 120e3 ) {
  print int( $timestamp ), "\n";
  $timestamp += 1 / $good_packets_per_millisecond;
}

my $bad_Mbps = 0.2;
my $bad_packets_per_millisecond = Mbps_to_ppms $bad_Mbps;

while ( $timestamp <= 240e3 ) {
  print int( $timestamp ), "\n";
  $timestamp += 1 / $bad_packets_per_millisecond;
}
