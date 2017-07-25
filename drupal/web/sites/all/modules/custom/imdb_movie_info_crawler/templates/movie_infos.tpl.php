<h1><?php print !empty($infos['title']) ? $infos['title'] : t("No title found!") ; ?></h1>
<?php if (!empty($infos['postersrc'])): ?>
    <div>
        <a href="<?php print $infos['posterlink']; ?>" >
            <img alt='movie poster'  src="<?php print print !empty($infos['postersrc']) ? $infos['postersrc'] : "" ; ?>"/>
        </a>
    </div>
<?php endif; ?>    
</br>
<p><?php print !empty($infos['plot']) ? $infos['plot'] : "" ; ?></p>
</br>
<h1><?php print l(t("AGAIN"), "/imdb_crawler", array('attributes' => array('class' => 'back-link'))); ?></h1>