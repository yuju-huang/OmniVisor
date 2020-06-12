# libnbd Python bindings
# Copyright (C) 2010-2019 Red Hat Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

import nbd

h = nbd.NBD ()

try:
    # This will always throw an exception because the handle is not
    # connected.
    h.pread (0, 0)
except nbd.Error as ex:
    print ("string = %s" % ex.string)
    print ("errno = %s" % ex.errno)
    print ("errnum = %d" % ex.errnum)
    exit (0)

# If we reach here then we didn't catch the exception above.
exit (1)
