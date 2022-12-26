#include "cmdtlv.h"
#include "libcli.h"

int
node_callback_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable) {
  printf("%s() is called ...\n", __FUNCTION__);
  return 0;
}

int
validate_node_name(char *value) {
  printf("%s() is called with value = %s...\n", __FUNCTION__, value);
  return VALIDATION_SUCCESS; //Or VALIDATION_FAILED
}

#define CMDODE_SHOW_CODE 1

int
main(int argc, char **argv){

    init_libcli();
    param_t *show   = libcli_get_show_hook();
    param_t *debug  = libcli_get_debug_hook();
    param_t *config = libcli_get_config_hook();
    param_t *clear  = libcli_get_clear_hook();
    param_t *run    = libcli_get_run_hook();

    /*Show node <node-name>*/

    {
      /*show node*/
      static param_t node;
      init_param(&node,  //address of current param
                 CMD, //CMD for command param, LEAF for leaf param
                 "node", //name of the param, the string is what we display in the command line.
                 0, // callback : pointer to application function. Null in this case since 'show node' is not a complete command
                 0, //Applicable only for LEAF params, always null for CMD params.
                 INVALID, //always invalid for CMD params
                 0, //always invalid for CMD params
                 "Help : node"); //Help string
      libcli_register_param(show, &node); //Add node param as suboption of show param
      {
        /*show node <node-name>*/
        static param_t node_name;
        init_param(&node_name,  //address of current param
                 LEAF, //CMD for command param, LEAF for leaf param
                 0, //Always null for LEAF param
                 node_callback_handler, // since it's a complete cmd, it should invoke application routine. Pass the pointer to the routine here.
                 validate_node_name, /*Optional, can be null, this is also an application specific routine, and perform validation test 
                                      to the value entered by the user for this leaf param. If validation pass, then only node_callback_handler routine is invoked*/
                 STRING, //leaf param value type, node name is string, hence pass STRING
                 "node-name", //Applicable only for LEAF param. Give some name to leaf-param. It's the string that we will parse in application code to find the value passed by the user.
                 "Help : Node name"); //Help string
        libcli_register_param(&node, &node_name);
        set_param_cmd_code(&node_name, CMDODE_SHOW_CODE);
      }
    }

    support_cmd_negation(config);
    /*Do not add any param in config command tree after above line*/
    start_shell();
    return 0;
}
