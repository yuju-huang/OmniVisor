#!@sbindir@/nbdkit perl
# -*- perl -*-

=pod

=head1 NAME

nbdkit-tar-plugin - read and write files inside tar files without unpacking

=head1 SYNOPSIS

 nbdkit tar tar=FILENAME.tar file=PATH_INSIDE_TAR

=head1 EXAMPLES

=head2 Serve a single file inside a tarball

 nbdkit tar tar=file.tar file=some/disk.img
 guestfish --format=raw -a nbd://localhost

=head2 Opening a disk image inside an OVA file

The popular "Open Virtual Appliance" (OVA) format is really an
uncompressed tar file containing (usually) VMDK-format files, so you
could access one file in an OVA like this:

 $ tar tf rhel.ova
 rhel.ovf
 rhel-disk1.vmdk
 rhel.mf
 $ nbdkit -r tar tar=rhel.ova file=rhel-disk1.vmdk
 $ guestfish --ro --format=vmdk -a nbd://localhost

In this case the tarball is opened readonly (I<-r> option).  The
plugin supports write access, but writing to the VMDK file in the
tarball does not change data checksums stored in other files (the
C<rhel.mf> file in this example), and as these will become incorrect
you probably won't be able to open the file with another tool
afterwards.

=head1 DESCRIPTION

C<nbdkit-tar-plugin> is a plugin which can read and writes files
inside an uncompressed tar file without unpacking the tar file.

The C<tar> and C<file> parameters are required, specifying the name of
the uncompressed tar file and the exact path of the file within the
tar file to access as a disk image.

This plugin will B<not> work on compressed tar files.

Use the nbdkit I<-r> flag to open the file readonly.  This is the
safest option because it guarantees that the tar file will not be
modified.  Without I<-r> writes will modify the tar file.

The disk image cannot be resized.

=head2 Alternatives to the tar plugin

The tar plugin ought to be a filter so that you can extract files from
within tarballs hosted elsewhere (eg. using L<nbdkit-curl-plugin(1)>).
However this is hard to implement given the way that the L<tar(1)>
command works.

Nevertheless you can apply the same technique even to tarballs hosted
remotely, provided you can run L<tar(1)> on them first.  The trick is
to use the S<C<tar -tRvf>> options to find the block number of the
file of interest.  In tar files, blocks are 512 bytes in size, and
there is one hidden block used for the header, so you have to take the
block number, add 1, and multiply by 512.

For example:

 $ tar -tRvf disk.tar
 block 2: -rw-r--r-- rjones/rjones 105923072 2020-03-28 20:34 disk
 └──┬──┘                           └───┬───┘
 offset = (2+1)*512 = 1536           range

You can then apply the offset filter:

 nbdkit --filter=offset \
          curl https://example.com/disk.tar \
               offset=1536 range=105923072

If the remote file is compressed then add L<nbdkit-xz-filter(1)>:

 nbdkit --filter=offset --filter=xz \
          curl https://example.com/disk.tar.xz \
               offset=1536 range=105923072

=head1 VERSION

C<nbdkit-tar-plugin> first appeared in nbdkit 1.2.

=head1 SEE ALSO

L<https://github.com/libguestfs/nbdkit/blob/master/plugins/tar/tar.pl>,
L<nbdkit(1)>,
L<nbdkit-offset-filter(1)>,
L<nbdkit-plugin(3)>,
L<nbdkit-perl-plugin(3)>,
L<nbdkit-xz-filter(1)>,
L<tar(1)>.

=head1 AUTHORS

Richard W.M. Jones.

Based on the virt-v2v OVA importer written by Tomáš Golembiovský.

=head1 COPYRIGHT

Copyright (C) 2017-2020 Red Hat Inc.

=cut

use strict;

use Cwd qw(abs_path);
use IO::File;

my $tar;                        # Tar file.
my $file;                       # File within the tar file.
my $offset;                     # Offset within tar file.
my $size;                       # Size of disk image within tar file.

sub config
{
    my $k = shift;
    my $v = shift;

    if ($k eq "tar") {
        $tar = abs_path ($v);
    }
    elsif ($k eq "file") {
        $file = $v;
    }
    else {
        die "unknown parameter $k";
    }
}

# Check all the config parameters were set.
sub config_complete
{
    die "tar or file parameter was not set\n"
        unless defined $tar && defined $file;

    die "$tar: file not found\n"
        unless -f $tar;
}

# Find the extent of the file within the tar file.
sub get_ready
{
    open (my $pipe, "-|", "tar", "--no-auto-compress", "-tRvf", $tar, $file)
        or die "$tar: could not open or parse tar file, see errors above";
    while (<$pipe>) {
        if (/^block\s(\d+):\s\S+\s\S+\s(\d+)/) {
            # Add one for the tar header, and multiply by the block size.
            $offset = ($1 + 1) * 512;
            $size = $2;
            Nbdkit::debug ("tar: file: $file offset: $offset size: $size")
        }
    }
    close ($pipe);

    die "offset or size could not be parsed.  Probably the tar file is not a tar file or the file does not exist in the tar file.  See any errors above.\n"
        unless defined $offset && defined $size;
}

# Accept a connection from a client, create and return the handle
# which is passed back to other calls.
sub open
{
    my $readonly = shift;
    my $mode = "<";
    $mode = "+<" unless $readonly;
    my $fh = IO::File->new;
    $fh->open ($tar, $mode) or die "$tar: open: $!";
    $fh->binmode;
    my $h = { fh => $fh, readonly => $readonly };
    return $h;
}

# Close the connection.
sub close
{
    my $h = shift;
    my $fh = $h->{fh};
    $fh->close;
}

# Return the size.
sub get_size
{
    my $h = shift;
    return $size;
}

# Read.
sub pread
{
    my $h = shift;
    my $fh = $h->{fh};
    my $count = shift;
    my $offs = shift;
    $fh->seek ($offset + $offs, 0) or die "seek: $!";
    my $r;
    $fh->read ($r, $count) or die "read: $!";
    return $r;
}

# Write.
sub pwrite
{
    my $h = shift;
    my $fh = $h->{fh};
    my $buf = shift;
    my $count = length ($buf);
    my $offs = shift;
    $fh->seek ($offset + $offs, 0) or die "seek: $!";
    print $fh ($buf);
}
