<?php

/**
 * @file ua_assign_role.admin.inc
 *
 * Administration form for User-agent Assign Role.
 */

/**
 * Callback for admin form.
 */
function ua_assign_role_admin() {
  $form = array();
  $form['ua_assign_role_user_agents'] = array(
    '#type' => 'textarea',
    '#title' => t('User-agents'),
    '#description' => t('A list of user-agents that are allowed to bypass the page cache. Enter one value per line. Values are case-sensitive. You may specify a partial value to match the first part of a user-agent name. EG: "facebookexternal" will match "facebookexternalhit/1.1".'),
    '#default_value' => implode("\n", variable_get('ua_assign_role_user_agents')),
    '#required' => TRUE,
  );
  $form['ua_assign_role_subnets'] = array(
    '#type' => 'textarea',
    '#title' => t('Permitted subnets'),
    '#description' => t('Optional. List of IPV4 or IPV6 subnets in CIDR format. Enter one value per line. Leave empty to match all. EG: "192.168.1.0/24" or "FD12:3456:789A:BCDE::/64".'),
    '#default_value' => implode("\n", variable_get('ua_assign_role_subnets')),
    '#required' => FALSE,
  );
  $role = variable_get('ua_assign_role_role');
  reset($role);
  $rid = key($role);
  $form['ua_assign_role_role'] = array(
    '#type' => 'select',
    '#title' => t('Role'),
    '#description' => t(
      'The role (!add) to be assigned to the visitor when the user-agent matches. <em>Warning: assign a role with only the required extra permissions. This has serious security implications.</em>',
      array('!add' => l('add new', 'admin/people/permissions/roles'))),
    '#default_value' => $rid,
    '#options' => user_roles(TRUE),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Validation function for ua_assign_role_admin().
 */
function ua_assign_role_admin_validate($form, &$form_state) {
  $form_state['values']['ua_assign_role_user_agents'] = _ua_assign_role_admin_explode_trim($form_state['values']['ua_assign_role_user_agents']);
  $form_state['values']['ua_assign_role_subnets'] = _ua_assign_role_admin_explode_trim($form_state['values']['ua_assign_role_subnets']);
  $rid = $form_state['values']['ua_assign_role_role'];
  $roles = user_roles();
  $form_state['values']['ua_assign_role_role'] = array(
    $rid => $roles[$rid],
  );
}

/**
 * Explode user-generated values separated by newlines. Trim values for whitespace and skip blank lines.
 *
 * @param string $value
 *    User entered values separated by newlines.
 * @return array
 *    Exploded and trimmed values as an array
 */
function _ua_assign_role_admin_explode_trim($value) {
  $values = explode("\n", $value);
  array_walk($values, function(&$item) {
    $item = trim($item);
  });
  return array_filter($values);
}