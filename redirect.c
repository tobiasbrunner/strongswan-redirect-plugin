/*
 * Copyright (C) 2016-2018 Tobias Bruner
 * HSR Hochschule fuer Technik Rapperswil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <library.h>
#include <plugins/plugin.h>

#include <daemon.h>
#include <sa/redirect_provider.h>

typedef struct private_redirect_plugin_t private_redirect_plugin_t;
typedef struct private_provider_t private_provider_t;

/**
 * Data for the plugin
 */
struct private_redirect_plugin_t {
	plugin_t public;
	private_provider_t *provider;
};

/**
 * Data for the redirection provider
 */
struct private_provider_t {
	redirect_provider_t public;
};

METHOD(redirect_provider_t, redirect_on_init, bool,
	private_provider_t *this, ike_sa_t *ike_sa,	identification_t **gateway)
{
	/* called to determine whether an IKE_SA is to be redirected during
	 * IKE_SA_INIT, example code:
	 *
	 * if (redirect_peer_with_ip(ike_sa->get_other_host(ike_sa)))
	 * {
	 *     *gateway = identification_create_from_string("<other gateway>");
	 *     return TRUE;
	 * }
	 * return FALSE;
	 */
	return FALSE;
}

METHOD(redirect_provider_t, redirect_on_auth, bool,
	private_provider_t *this, ike_sa_t *ike_sa,	identification_t **gateway)
{
	/* called to determine whether an IKE_SA is to be redirected during
	 * IKE_AUTH, example code:
	 *
	 * if (redirect_peer_with_id(ike_sa->get_other_eap_identity(ike_sa)))
	 * {
	 *     *gateway = identification_create_from_string("<other gateway>");
	 *     return TRUE;
	 * }
	 * return FALSE;
	 */
	return FALSE;
}

static void destroy_provider(private_provider_t *this)
{
	free(this);
}

private_provider_t *provider_create()
{
	private_provider_t *this;

	INIT(this,
		.public = {
			/* both of these are optional to implement/assign */
			.redirect_on_init = _redirect_on_init,
			.redirect_on_auth = _redirect_on_auth,
		},
	);
	return this;
}

/* --------- Plugin --------- */

METHOD(plugin_t, get_name, char*,
	private_redirect_plugin_t *this)
{
	return "redirect";
}

static bool plugin_cb(private_redirect_plugin_t *this,
					  plugin_feature_t *feature, bool reg, void *cb_data)
{
	if (reg)
	{
		charon->redirect->add_provider(charon->redirect,
									   &this->provider->public);
	}
	else
	{
		charon->redirect->remove_provider(charon->redirect,
										  &this->provider->public);
	}
	return TRUE;
}

METHOD(plugin_t, get_features, int,
	private_redirect_plugin_t *this, plugin_feature_t *features[])
{
	static plugin_feature_t f[] = {
		PLUGIN_CALLBACK((plugin_feature_callback_t)plugin_cb, NULL),
			PLUGIN_PROVIDE(CUSTOM, "redirect"),
	};
	*features = f;
	return countof(f);
}

METHOD(plugin_t, destroy, void,
	private_redirect_plugin_t *this)
{
	destroy_provider(this->provider);
	free(this);
}

plugin_t *redirect_plugin_create()
{
	private_redirect_plugin_t *this;

	INIT(this,
		.public = {
			.get_name = _get_name,
			.get_features = _get_features,
			.destroy = _destroy,
		},
		.provider = provider_create(),
	);

	return &this->public;
}
