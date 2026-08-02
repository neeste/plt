#!/usr/bin/perl -w
use strict;
use warnings;
#
# declare local variables
#
my $pgm = "plt";
my $PGM = uc $pgm;
my $n;
my @infile;
my $ver;
my $vdt;
my $i;
#
# get version number from version.h
#
$n = "../tplt/version.h";
open FILE,$n or die "can't open ".$n;
@infile = <FILE>;
foreach(@infile)
{
    if($_ =~ /#define\sREV.*/) {
	($ver) = $_ =~ /.*(\d\.\d\d[a-z]*), .*"/;
	($vdt) = $_ =~ /.*\d\.\d\d[a-z]*, (.*)"/;
    }
}
close (FILE);
print "version is $ver on $vdt\n";

# Removed legacy updates for Info.plist and MacPlot.xcodeproj

