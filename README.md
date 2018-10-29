# Example Plugin for strongSwan's IKEv2 Redirection Functionality (RFC 5685)

Very simple out-of-tree strongSwan plugin that shows the use of the
[`redirect_provider_t`](https://github.com/strongswan/strongswan/blob/master/src/libcharon/sa/redirect_provider.h)
interface that may be used to redirect IKEv2 clients to another gateway using
the protocol specified in [RFC 5685](http://tools.ietf.org/html/rfc5685).

The two methods of that interface, `redirect_on_init()` and `redirect_on_auth()`,
are both optional to implement and allow redrection during `IKE_SA_INIT` and/or
`IKE_AUTH`, respectively. Note that it's also possible to redirect clients later
e.g. via [VICI](https://wiki.strongswan.org/projects/strongswan/wiki/Vici)'s
[`redirect` command](https://github.com/strongswan/strongswan/blob/master/src/libcharon/plugins/vici/README.md#redirect).

The provided Makefile can be configured on the command line with some variables
that specify the location of several things required for the build and later
during the installation of the plugin.

Since it's an out-of-tree plugin, the daemon won't load it, by default. The
provided config snippet assumes that [the modular plugin configuration](https://wiki.strongswan.org/projects/strongswan/wiki/PluginLoad#Modular-Configuration)
is in use.
