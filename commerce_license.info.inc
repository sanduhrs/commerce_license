<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

class CommerceLicenseMetadataController extends EntityDefaultMetadataController {

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = array();
    $properties = &$info['commerce_license']['properties'];

    $properties['license_id'] = array(
      'type' => 'integer',
      'label' => t('License ID'),
      'description' => t('The primary identifier for a license.'),
      'validation callback' => 'entity_metadata_validate_integer_positive',
      'schema field' => 'license_id',
    );
    $properties['type'] = array(
      'type' => 'token',
      'label' => t('Type'),
      'description' => t('The license type'),
      'options list' => 'CommerceLicenseMetadataController::bundleOptionsList',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'schema field' => 'type',
    );
    $properties['owner'] = array(
      'type' => 'user',
      'label' => t('Owner'),
      'description' => t('The owner of the license.'),
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'schema field' => 'uid',
    );
    $properties['product'] = array(
      'type' => 'commerce_product',
      'label' => t('Product'),
      'description' => t('The licensed product.'),
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'schema field' => 'product_id',
    );
    $properties['status'] = array(
      'type' => 'integer',
      'label' => t('Status'),
      'description' => t('The license status.'),
      'options list' => 'commerce_license_status_options_list',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'schema field' => 'status',
    );
    $properties['granted'] = array(
      'type' => 'date',
      'label' => t('Granted'),
      'description' => t('The date when the license was granted.'),
      'schema field' => 'granted',
    );
    $properties['expires'] = array(
      'type' => 'date',
      'label' => t('Expires'),
      'description' => t('The date when the license expires. 0 for never.'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'expires',
    );
    $properties['revised_by'] = array(
      'type' => 'user',
      'label' => t('Revised by'),
      'description' => t('The creator of the license revision.'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'revision_uid',
    );
    $properties['revision_created'] = array(
      'type' => 'date',
      'label' => t('Revision created'),
      'description' => t('The date when the license revision was created.'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'revision_created',
    );
    $properties['revision_ended'] = array(
      'type' => 'date',
      'label' => t('Revision ends'),
      'description' => t('The date when the license revision ended, if any.'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'revision_ended',
    );
    $properties['log'] = array(
      'type' => 'text',
      'label' => t('Revision log'),
      'description' => t('The log message for this license revision, if any.'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'log',
    );
    // entity_token uses this to provide [commerce_license:access-details].
    $properties['access_details'] = array(
      'label' => t('Access details'),
      'description' => t('The access details of an activated license.'),
      'getter callback' => 'commerce_license_get_access_details',
    );

    return $info;
  }
}
