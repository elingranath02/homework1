void enterBathroom(int id) {
    int myGender = id % 2;  // 0 = kvinna, 1 = man

    sem_wait(waitroomLock);

    if (countInBathroom == 0) {
        // Om badrummet är tomt, sätt könet och lås det
        genderInBathroom = myGender;
        printf("Badrummet öppnas för %s.\n", myGender == 0 ? "kvinnor" : "män");
        sem_wait(bathroomLock);
    } else if (myGender != genderInBathroom) {
        // Vänta om det motsatta könet är i badrummet
        countInQueue++;
        printf("Person %d (%s) väntar på badrummet.\n", id,
               myGender == 0 ? "kvinna" : "man");
        sem_post(waitroomLock);  // Släpp kön så andra kan komma in
        sem_wait(bathroomLock);  // Vänta på tillträde
        sem_wait(waitroomLock);  // Lås igen när det är dags att gå in
        countInQueue--;
    }

    // Gå in i badrummet
    countInBathroom++;
    printf("Person %d (%s) använder badrummet.\n", id,
           myGender == 0 ? "kvinna" : "man");
    sem_post(waitroomLock);  // Släpp kön så andra kan gå in

    sleep(1);  // Simulera badrumsanvändning

    // Klart i badrummet
    sem_wait(waitroomLock);
    printf("Person %d (%s) är klar.\n", id, myGender == 0 ? "kvinna" : "man");
    countInBathroom--;

    if (countInBathroom == 0) {
        printf("Badrummet är tomt.\n");
        genderInBathroom = -1;   // Återställ kön
        sem_post(bathroomLock);  // Släpp in nästa grupp
    }

    sem_post(waitroomLock);
}
