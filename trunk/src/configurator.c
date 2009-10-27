/*
   (c) Copyright 2007  AvengerGear.com

   All rights reserved.

   This utility reference src/misc/conf.h file in directfb source
   Please reference http://www.directfb.org

   Modified by Alex Lau <avengermojo@gmail.com> 

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <configurator.h>

/*
 * global configuration pointer for ued future use.
 */
UEDConfigurator *ued_config = NULL;

static const char *config_usage =
    "Universal Encoding Detector version " UED_VERSION "\n\n"
    "  -o:<option>[,<option>]		              Pass different options to UED as below\n"
    "  -help				              Output this menu\n"
    "UED options:\n"
    "\n"
    "  disable-module=<module_name>[,<module_name>]   Specify the modules(Unicode, etc.)\n"
    "\n";

/*
 * allocates configurator and fill in some default value
 */
static void
config_allocate()
{
    UED_DEBUG_ENTER( CONFIGURATOR , "Allocating configurator memory" );
    if (ued_config) {
        UED_DEBUG_EXIT( CONFIGURATOR , "Configurator has already been called" );
        return;
    }
    ued_config = (UEDConfigurator*) calloc( 1, sizeof(UEDConfigurator) );
    ued_config->enable = true;
    ued_config->disable_module = NULL;
    UED_DEBUG_EXIT( CONFIGURATOR , "Configurator being allocated successfully" );
}

/*
 * parse different argurment into the configurator
 */
static UEDResult
parse_args( const char *args )
{
    UED_DEBUG_ENTER( CONFIGURATOR , "Parsing different argruments" );
	UEDResult ret=UED_OK;
    char* buf = alloca( strlen(args) + 1 );

    strcpy( buf, args );

    while (buf && buf[0]) {
		char* value = NULL;
		char* next = NULL;

        if ((next = strchr( buf, ',' )) != NULL)
            *next++ = '\0';
        if ((value = strchr( buf, '=' )) != NULL)
            *value++ = '\0';
        ret = ued_config_set( buf, value );
        switch (ret) {
            case UED_OK: break;
            case UED_UNSUPPORTED:
                UED_DEBUG( "Configur: Unknown option '%s'!", buf );
                break;
            default:
                UED_DEBUG_EXIT( CONFIGURATOR , "Parser unknown stage" );
                return ret;
        }
    }
    UED_DEBUG_EXIT( CONFIGURATOR , "Parser finish successfully" );

	return ret;
}

/*
 * Allocate Configuration struct, fill with defaults and parse command line 
 * options for overrides.
 */
UEDResult 
ued_config_init( int *argc, char *(*argv[]) )
{
    UED_DEBUG_ENTER( CONFIGURATOR , "Initializing and start to read different configuration" );
    UEDResult ret=UED_OK;
    char *home = getenv( "HOME" );
    char *env_args;
    int  i;

    /* make sure it won't be call twice */
    if (ued_config) {
        UED_DEBUG_EXIT( CONFIGURATOR , "Configurator already been created" );
        return ret;
    }

    config_allocate();

    /* Read system settings. */
    ret = ued_config_read( "/etc/uedrc" );
    if (ret != UED_OK ){
        UED_DEBUG_EXIT( CONFIGURATOR , "Can not read system /etc/uedrc configuration file!" );
        return ret;
    }

    /* Read user settings. */
    if (home) {
        int  len = strlen(home) + strlen("/.uedrc") + 1;
        char buf[len];
        snprintf( buf, len, "%s/.uedrc", home );
        ret = ued_config_read( buf );
        if (ret != UED_OK) {
            UED_DEBUG_EXIT( CONFIGURATOR , "Can not read user $HOME/.uedrc configuration file!" );
            return ret;
        }
    }

    /* Read settings from environment variable. */
    env_args = getenv( "UEDCONF" );
    if (env_args) {
        ret = parse_args( env_args );
        if (ret) {
            UED_DEBUG_EXIT( CONFIGURATOR , "Can not read user enviornment variable UEDCONF" );
            return ret;
        }
    }

    /* Read settings from command line. */
    if (argc && argv) {
        for (i = 1; i < *argc; i++) {
            if (strcmp ((*argv)[i], "-help") == 0) {
                fprintf( stderr, config_usage );
                exit(1);
            } else if (strncmp ((*argv)[i], "-o:", 3) == 0) {
                ret = parse_args( (*argv)[i] + 3 );
                if (ret) {
                    UED_DEBUG_EXIT( CONFIGURATOR , "Command line configurator option error" );
                    return ret;
                }
	        (*argv)[i] = NULL;
            }
        }
    }
    UED_DEBUG_EXIT( CONFIGURATOR , "Configuration initialize complete successfully" );
    return UED_OK;
}

/*
 * Read configuration options from file. Called by ued_config_init().
 */
UEDResult
ued_config_read( const char *filename )
{
    return UED_OK;
}


/*
 * Set indiviual option. Used by ued_config_init(), ued_config_read()
 *
 * TODO: 
 * does it need to be thread safe? 
 */
UEDResult 
ued_config_set( const char *name, const char *value )
{
    UED_DEBUG_ENTER( CONFIGURATOR , "setting different configuration value" );
    if (!ued_config) { 
        UED_DEBUG_EXIT( CONFIGURATOR , "configurator is not initialized!" );
        return UED_FAILURE;
    }
    if (strcmp (name, "disable-module" ) == 0) {
        if (value) {
            int n = 0, i;
            while (ued_config->disable_module &&
                   ued_config->disable_module[n])
                n++;
            if( ued_config->disable_module ) {
                char **buf = calloc( 1, sizeof(char*) * (n + 2) );
                for( i = 0; i < n; i++ ){
                    buf[i]=strdup( ued_config->disable_module[i] );
                    free(ued_config->disable_module[i]);
                }
                free(ued_config->disable_module);
                ued_config->disable_module = buf;
            } else {
                ued_config->disable_module = calloc( 1, sizeof(char*) * (n + 2) );
            }
            ued_config->disable_module[n] = strdup( value );
            ued_config->disable_module[n+1] = NULL;
        } else {
            UED_DEBUG_EXIT( CONFIGURATOR , "'disable_module': No module name specified!" );
            return UED_INVARG;
        }
    } else { 
            UED_DEBUG_EXIT( CONFIGURATOR , "%s is not valide argurment!", name );
            return UED_UNSUPPORTED;
    }
    UED_DEBUG_EXIT( CONFIGURATOR , "configuration being set successfully!" );
    return UED_OK;
}

/*
 * Provide how to use the configuration file and description
 */
const char *ued_config_usage( void )
{
     return config_usage;
}
