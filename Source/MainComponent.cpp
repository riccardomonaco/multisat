#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Nota: Sostituisci questo percorso con quello effettivo in cui si trova scsynth sul tuo sistema.
    // In fase di release, questo percorso sarà relativo alla tua applicazione.
    juce::String scPath = "/Applications/SuperCollider.app/Contents/Resources/scsynth";
    juce::StringArray scArgs{ scPath, "-u", "57110" }; // 57110 è la porta di ascolto UDP standard di scsynth

    // 2. Lancia il server in background
    if (scServer.start(scArgs)) {
        DBG("SuperCollider avviato con successo.");
    }
    else {
        DBG("ERRORE: Impossibile avviare SuperCollider. Controlla il percorso.");
    }

    // 3. Connetti il modulo OSC di JUCE alla porta appena aperta
    if (!oscSender.connect("127.0.0.1", 57110)) {
        DBG("ERRORE: Impossibile creare il socket OSC.");
    }

    playButton.setButtonText("Avvia Suono");
    playButton.onClick = [this] {
        // 1. Carica il file binario compilato.
        // ATTENZIONE: Usa il path assoluto corretto del file .scsyndef sul tuo disco
        oscSender.send("/d_load", "/percorso/assoluto/verso/terrain.scsyndef");

        // 2. Istanzia il Synth
        // Argomenti: nome_synth, NodeID (1000), AddAction (1 = tail), TargetID (0 = root)
        // Aggiungiamo anche il parametro "amp" a 0.5 per sicurezza
        oscSender.send("/s_new", "terrain", 1000, 1, 0, "amp", 0.5f);
        };

    // Rende il componente visibile sulla GUI
    addAndMakeVisible(playButton);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{

    if (scServer.isRunning()) {
        scServer.kill();
        DBG("SuperCollider terminato.");
    }
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // 1. Pulisce lo sfondo riempiendolo di un colore (es. grigio scuro)
    g.fillAll(juce::Colours::darkgrey);

    // 2. Imposta il colore del testo (es. bianco)
    g.setColour(juce::Colours::white);

    // 3. Imposta la dimensione e lo stile del font
    g.setFont(juce::Font(30.0f, juce::Font::bold));

    // 4. Disegna il testo al centro della finestra
    g.drawText("It feels so empty in here...",
        getLocalBounds(),        // L'area in cui disegnare (tutta la finestra)
        juce::Justification::centred, // Allineamento
        true);                   // Aggiunge i puntini di sospensione se il testo è troppo lungo
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
