<ol>
    <?php
    foreach ($movietitles as $title) {
        print "<h2><li>" . $title . "</li></h2>";
    }
    ?>   
</ol>
<h3><?php print "Last updated: ".$time_of_update_markup;?></li></h3>