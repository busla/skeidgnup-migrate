<?php

abstract class SkeidgnupMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    // The example uses a SQL database as source.
      $this->team = array(
      new MigrateTeamMember('Jón Levy', 'zazen@riseup.net', t('Drupal og migrate kóði')),
    );
  }
}


class SkeidgnupNewsMigration extends SkeidgnupMigration {
  public function __construct($arguments) {
  parent::__construct($arguments);

    $query = db_select('news', 'nt')
             ->fields('nt', array('NewsID', 'Subject', 'Date'));
    $query->join('news_lng', 'nb', 'nb.NewsID = nt.NewsID');
    $query->addField('nb', 'Content', 'body');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('post');
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'NewsID' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                          'alias' => 'nt',
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );
    $this->addFieldMapping('title', 'Subject');
    $this->addFieldMapping('created', 'Date');
    $this->addFieldMapping('field_body', 'body')
         ->arguments(array('format' => 'filtered_html'));
    $this->addFieldMapping('field_category')
         ->arguments(array('source_type' => 'tid'))
         ->defaultValue(6);
    $this->addFieldMapping('field_settings')
         ->arguments(array('source_type' => 'tid'))
         ->defaultValue(5);
    $this->addFieldMapping('uid')
         ->defaultValue(3);
        
  }
}

class SkeidgnupEventsMigration extends SkeidgnupMigration {
  public function __construct($arguments) {
  parent::__construct($arguments);

    $query = db_select('events', 'e')
             ->fields('e', array('EventID', 'Subject', 'Date'));
    $query->join('events_lng', 'el', 'el.EventID = e.EventID');
    $query->addField('el', 'Content', 'body');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('post');
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'EventID' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                          'alias' => 'e',
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );
    $this->addFieldMapping('title', 'Subject');
    $this->addFieldMapping('created', 'Date');
    $this->addFieldMapping('field_body', 'body')
         ->arguments(array('format' => 'filtered_html'));
    $this->addFieldMapping('field_settings')
         ->arguments(array('source_type' => 'tid'))
         ->defaultValue(array(4, 5,));
    $this->addFieldMapping('field_category')
         ->arguments(array('source_type' => 'tid'))
         ->defaultValue(6);
    $this->addFieldMapping('uid')
         ->defaultValue(3);
        
  }
}


class SkeidgnupPagesMigration extends SkeidgnupMigration {
  public function __construct($arguments) {
  parent::__construct($arguments);
    $query = db_select('pages_lng', 'p')
             ->fields('p', array('PageID', 'pageTitle', 'pageContent', 'Place'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('post');
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'PageID' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );
//        $this->addFieldMapping('title', 'pageTitle');
          $this->addFieldMapping('title', 'pageTitle');
          $this->addFieldMapping('field_body', 'pageContent')
               ->arguments(array('format' => 'filtered_html'));
          $this->addFieldMapping('uid')
               ->defaultValue(3);
               
  }
    public function prepareRow($row) {
  // Always include this fragment at the beginning of every prepareRow()
  // implementation, so parent classes can ignore rows.
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  // If marked as spam in the related data, skip this row
    if ($row->Place == 1) {
      return FALSE;
    }
    
    return TRUE;
  }
  
}

class SkeidgnupPlacesMigration extends SkeidgnupMigration {
  public function __construct($arguments) {
  parent::__construct($arguments);
    $query = db_select('pages_lng', 'p')
             ->fields('p', array('PageID', 'pageTitle', 'pageContent', 'Place'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('place');
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'PageID' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );
//        $this->addFieldMapping('title', 'pageTitle');
          $this->addFieldMapping('title', 'pageTitle');
          $this->addFieldMapping('field_body', 'pageContent')
               ->arguments(array('format' => 'filtered_html'));
          $this->addFieldMapping('uid')
               ->defaultValue(3);
               
  }
  
  public function prepareRow($row) {
  // Always include this fragment at the beginning of every prepareRow()
  // implementation, so parent classes can ignore rows.
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  // If marked as spam in the related data, skip this row
    if ($row->Place == 0) {
      return FALSE;
    }
    
    return TRUE;
  }
  
}

class SkeidgnupMeetingsMigration extends SkeidgnupMigration {
  public function __construct($arguments) {
    $this->sourceFields['new_board'] = t('Nýr nefndarflokkur');
    parent::__construct($arguments);

    $query = db_select('meetings', 'm')
             ->fields('m', array('MeetingID', 'Date', 'Subject'));
    $query->join('meetings_lng', 'ml', 'm.MeetingID = ml.MeetingID');
    $query->addField('ml', 'Content', 'body');
    $query->addField('ml', 'CategoryID', 'board');    

    $source_fields = array('new_board' => t('Nýr nefndarflokkur'));
    $this->source = new MigrateSourceSQL($query, $source_fields);
    
    $this->destination = new MigrateDestinationNode('reports');
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'MeetingID' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                          'alias' => 'm',
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );
    $this->addFieldMapping('title', 'Subject');
    $this->addFieldMapping('created', 'Date');
    $this->addFieldMapping('field_body', 'body')
         ->arguments(array('format' => 'filtered_html'));
    $this->addFieldMapping('field_board_type', 'new_board')
         ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_category')
         ->arguments(array('source_type' => 'tid'))
         ->defaultValue(10);
    $this->addFieldMapping('uid')
         ->defaultValue(3);
         
        
  }
  
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;    
    }
    
    switch ($row->board) {
    case 1: //Hreppsnefnd
        $row->new_board = 15;
        break;
    case 2: // Skólanefnd
        $row->new_board = 16;
        break;
    case 3: // Umhverfisnefnd
        $row->new_board = 17;
        break;
    case 6: // Aðrar nefndir
        $row->new_board = 35;
        break;
    }

    return TRUE;
  }  
}

