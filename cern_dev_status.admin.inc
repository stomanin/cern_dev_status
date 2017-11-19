<?php

// $Id$

/**
 * @file: Form builder; display the cern_dev_status settings form.
 *
 * @see cern_dev_status_settings_form_submit()
 */


/**
 * Page callback function for admin/config/development/cern_dev_status
 */

function cern_dev_status_settings_form($form, &$form_state) { 
  $form['cern_dev_status_description'] = array(
    '#markup' => '<h2>' . t('Select the options to enable') . '</h2><p><strong>' . t("This options should be enabled on development sites only!") . '</strong></p>' ,
  );  
  $form['cern_dev_status_enable_noindex'] = array(
  '#type' => 'checkbox',
  '#title' => t('Avoid indexing'),
  '#description' => t('Add &lt;meta name&#61;&quot;robots&quot; content&#61;&quot;noindex&quot; &#47;&gt; to the head of all pages avoinding automated Internet bots indexing.'),    
  '#default_value' => variable_get('cern_dev_status_enable_noindex', DEFAULT_ENABLE_NOINDEX),
  );
  
  $form['cern_dev_status_enable_devstyling'] = array(
  '#type' => 'checkbox',
  '#title' => t('Apply dev-status styling to website'),
  '#description' => t('Add a class to the &#36;classes array that can be used to apply styles via the cern_default theme.<br />These styles should be used on development and pre-production sites only.'),    
  '#default_value' => variable_get('cern_dev_status_enable_devstyling', DEFAULT_ENABLE_DEVSTYLING),
  );
  $form['cern_dev_status_devstyling_type'] = array(
  '#type' => 'radios',
  '#title' => t('Choose the style to apply:'),
  '#required' => TRUE,
  '#default_value' => variable_get('cern_dev_status_devstyling_type', DEFAULT_DEVSTYLING_TYPE),
  '#options' => array('dev' => t('Development (class name: <em>dev-status-dev</em>)'), 
                      'test' => t('Test (class name: <em>dev-status-test</em>)'), 
                      'preprod' => t('Pre-production (class name: <em>dev-status-preprod</em>)')
                      ),
  '#states' => array(
    'visible' => array(
      ':input[name="cern_dev_status_enable_devstyling"]' => array('checked' => TRUE),
    ),
   ),

  );
  $form['cern_dev_status_devstyling_message'] = array(
  '#type' => 'textarea',
  '#title' => t('Status message'),
  '#description' => t('This message will be displayed on the bottom of each page.'),
  '#default_value' => variable_get('cern_dev_status_devstyling_message'),
  '#states' => array(
    'visible' => array(
      ':input[name="cern_dev_status_enable_devstyling"]' => array('checked' => TRUE),
    ),
   ),
  );
  
  $form['cern_dev_status_enable_restrict_ip'] = array(
  '#type' => 'checkbox',
  '#title' => t('Enable IP restrictions'),
  '#description' => t('Restrict access for anonymous users to specified IP addresses (CERN IP ranges are set as default).'),    
  '#default_value' => variable_get('cern_dev_status_enable_restrict_ip', DEFAULT_ENABLE_RESTRICTIP),
  );
  
  
  
  $form['cern_dev_status_restrict_ip'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Enter the list of allowed IP addresses below'),
    '#states' => array(
    // Only make visible this fieldset when the restrict_ip option is enabled.
    'visible' => array(
      ':input[name="cern_dev_status_enable_restrict_ip"]' => array('checked' => TRUE),
    ),
   ),
  );

    $form['cern_dev_status_restrict_ip']['restrict_ip_address_description'] = array
    (
      '#markup' => '<p><strong>' . t('Your current IP address is: !ip_address', array('!ip_address' => '<em>' . ip_address() . '</em>')) . '</strong></p>',
  //    '#markup' => '<p><strong style="color:red">' . t("Warning: If you don't enter your current IP address into the list, you will immediately be locked out of the system upon save, and will not be able to access the system until you are in a location with an allowed IP address.") . '</strong></p><p><strong>' . t('Your current IP address is: !ip_address', array('!ip_address' => '<em>' . ip_address() . '</em>')) . '</strong></p>',
    );
    $form['cern_dev_status_restrict_ip']['cern_dev_status_restrict_ip_address_list'] = array
    (
      '#title' => t('Allowed IP Address List'),
      '#description' => t('Enter the list of IP Addresses that are allowed to access the site. If this field is left empty, all IP addresses will be able to access the site. Enter one IP address per line. You may also enter a range of IP addresses in the CIDR format. ex. xxx.xxx.xxx.xxx/xx'),
      '#type' => 'textarea',
      '#default_value' => variable_get('cern_dev_status_restrict_ip_address_list', ip_address().PHP_EOL.DEFAULT_IP_ADDDRESS_LIST), //add the admin's ip address with the default list
    ); 
     $form['cern_dev_status_restrict_ip']['cern_dev_status_restrict_ip_override_auth_users'] = array
     (
    '#type' => 'checkbox',
    '#title' => t('Override IP restrictions for authenticated users'),
    '#description' => t('Allow authenticated users to login even if their IP address is not included in the Allowed IP Address List above.'),    
    '#default_value' => variable_get('cern_dev_status_restrict_ip_override_auth_users', DEFAULT_ENABLE_OVERRIDE_AUTH_USERS),
    );

    $form['cern_dev_status_restrict_ip']['cern_dev_status_restrict_ip_mail_address'] = array
    (
      '#title' => t('Email Address'),
      '#type' => 'textfield',
      '#description' => t('If you would like to include a contact email address in the error message that is shown to users that do not have an allowed IP address, enter the email address here.'),
      '#default_value' => trim(variable_get('cern_dev_status_restrict_ip_mail_address', variable_get('site_mail', ''))),
    );


  
  
    $form['#submit'][] = 'cern_dev_status_settings_form_submit';

  return system_settings_form($form);
}
 
function cern_dev_status_settings_form_submit($form, &$form_state) {
  //enable or disable block 
  if ($form['cern_dev_status_enable_devstyling']['#value']) {
    db_update('block')->fields(array('status' => '1'))->condition('delta', 'cern_dev_status_devstyling')->execute();
    db_update('block')->fields(array('region' => 'page_bottom'))->condition('delta', 'cern_dev_status_devstyling')->execute();
  } 
  else {
    db_update('block')->fields(array('status' => '0'))->condition('delta', 'cern_dev_status_devstyling')->execute();
    db_update('block')->fields(array('region' => ''))->condition('delta', 'cern_dev_status_devstyling')->execute();
  }
}

 
/**
 * Validation function for cern_dev_status_settings_form()
 *
 * This function determines whether or not the values entered
 * in whitelisted IPs list are valid IP addresses
 */
 
function cern_dev_status_settings_form_validate($form, &$form_state) {
  $lockedout = TRUE;

  $ip_addresses = $form_state['values']['cern_dev_status_restrict_ip_address_list'];
  if (strlen(trim($ip_addresses))) {
    $ip_addresses = explode(PHP_EOL, trim($form_state['values']['cern_dev_status_restrict_ip_address_list']));
    foreach ($ip_addresses as $ip_address) {
      if (trim($ip_address) != '::1') { 
        //one ip
        if (!preg_match('~^\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b$~', trim($ip_address))) { 
          //ip range
          if (!preg_match('@\b(?:[0-9]{1,3}\.){3}[0-9]{1,3}\/(?:[12]?[0-9]|3[0-2])\b@', trim($ip_address))) {
            form_set_error('cern_dev_status_restrict_ip_address_list', t('@ip_address is not a valid range of IP addresses.', array('@ip_address' => $ip_address)));
          } 
          else {
            //$ip_address is an IP range, check if the admin's ip is included in it
            if (cern_dev_status_ipCIDRCheck(trim(ip_address()), $ip_address)) $lockedout = FALSE;
          }
        } 
        else {
          //$ip_address is a single ip, check if it's the admin's ip
          if (trim($ip_address) == ip_address()) $lockedout = FALSE;
        }
      } 
      else {
        //$ip_address is ::1, check if it's the admin's ip
        if (trim($ip_address) == ip_address()) $lockedout = FALSE;
      }
    }
  }
  if ($lockedout) {
    form_set_error('cern_dev_status_restrict_ip_address_list', t('Your IP address is not included in the list, cannot save the settings: you would be locked out of the site.'));  
  }
  
  $emaddr = $form_state['values']['cern_dev_status_restrict_ip_mail_address'];
  if (!valid_email_address($emaddr)) {
    form_set_error('cern_dev_status_restrict_ip_mail_address', t('@emaddr is not a valid email address.', array('@emaddr' => $emaddr)));
  }
 
  
}
 
 
