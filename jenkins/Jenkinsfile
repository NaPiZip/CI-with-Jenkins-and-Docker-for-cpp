pipeline {
    agent any
    stages {
        stage('preprocessing') {
            steps {
                echo "Preprocessing"
            }
        }
        stage('build') {
            steps {
                sh "./build_full.sh"
            }
        }
    }
}
