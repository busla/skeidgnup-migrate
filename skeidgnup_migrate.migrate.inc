<?php
function skeidgnup_migrate_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'skeidgnup' => array(
        'title' => t('Skeiðgnúp gamli vefur'),
      ),
    ),
    'migrations' => array(
      'SkeidgnupNews' => array(        
        'class_name' => 'SkeidgnupNewsMigration',
        'group_name' => 'skeidgnup',
      ),
      'SkeidgnupPages' => array(
        'class_name' => 'SkeidgnupPagesMigration',
        'group_name' => 'skeidgnup',
      ),
      'SkeidgnupMeetings' => array(
        'class_name' => 'SkeidgnupMeetingsMigration',
        'group_name' => 'skeidgnup',
      ),
      'SkeidgnupEvents' => array(
        'class_name' => 'SkeidgnupEventsMigration',
        'group_name' => 'skeidgnup',
      ),
      'SkeidgnupPlaces' => array(
        'class_name' => 'SkeidgnupPlacesMigration',
        'group_name' => 'skeidgnup',
      ),

    ),
  );
  return $api;
}