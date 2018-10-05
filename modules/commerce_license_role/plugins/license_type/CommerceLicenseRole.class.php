<?php

/**
 * Role license type.
 */
class CommerceLicenseRole extends CommerceLicenseBase  {

  /**
   * Implements CommerceLicenseInterface::isConfigurable().
   */
  public function isConfigurable() {
    return FALSE;
  }

  /**
   * Overrides Entity::save().
   *
   * Maintains the role, adding or removing it from the owner when necessary.
   */
  public function save() {
    if ($this->uid && $this->product_id) {
      $role = $this->wrapper->product->commerce_license_role->value();
      $owner = $this->wrapper->owner->value();
      $save_owner = FALSE;
      if (!empty($this->license_id)) {
        $this->original = entity_load_unchanged('commerce_license', $this->license_id);
        // A plan change occurred. Remove the previous role.
        if ($this->original->product_id && $this->product_id != $this->original->product_id) {
          $previous_role = $this->original->wrapper->product->commerce_license_role->value();
          if (isset($owner->roles[$previous_role])) {
            $this->revokeRole($owner, $previous_role);
            $save_owner = TRUE;
          }
        }
      }
      // The owner of an active license must have the role.
      if ($this->status == COMMERCE_LICENSE_ACTIVE) {
        if (!isset($owner->roles[$role])) {
          $owner->roles[$role] = $role;
          $save_owner = TRUE;
        }
      }
      elseif ($this->status > COMMERCE_LICENSE_ACTIVE) {
        // The owner of an inactive license must not have the role.
        if (isset($owner->roles[$role])) {
          $this->revokeRole($owner, $role);
          $save_owner = TRUE;
        }
      }

      // If a role was added or removed, save the owner.
      if ($save_owner) {
        user_save($owner);
      }
    }

    parent::save();
  }

  /**
   * Revokes the users role if there are no other active licenses.
   *
   * @param $owner object
   *   The Drupal user object.
   * @param $role integer
   *   The numeric role value.
   */
  private function revokeRole($owner, $role) {
    $revoke = TRUE;

    // Load all of the users active licenses that have yet to expire.
    $efq = new EntityFieldQuery();
    $results = $efq->entityCondition('entity_type', 'commerce_license')
      ->propertyCondition('type', 'role')
      ->propertyCondition('uid', $owner->uid)
      ->propertyCondition('status', COMMERCE_LICENSE_ACTIVE)
      ->propertyCondition('license_id', $this->license_id, '!=')
      ->execute();

    // Loop through all of the users active licenses to see if they have another
    // active license with the same role.
    if (!empty($results)) {
      $licenses = entity_load('commerce_license', array_keys($results['commerce_license']));
      foreach ($licenses as $license) {
        $license_wrapper = entity_metadata_wrapper('commerce_license', $license);
        try {
          $license_role = $license_wrapper->product->commerce_license_role->value();
        }
        catch (EntityMetadataWrapperException $ex) {
          $license_role = FALSE;
        }

        // Set the revoke flag to false if there is a match so the user keeps their role.
        if ($license_role == $role) {
          $revoke = FALSE;
        }
      }
    }

    // Revoke the user role if it exists and there isn't another license for the same user
    // and role.
    if ($revoke && isset($owner->roles[$role])) {
      unset($owner->roles[$role]);
    }
  }
}
