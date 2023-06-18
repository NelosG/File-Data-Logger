package file.logger

import file.logger.logger.Logger
import trash.remote.file.stats.StatsProvider
import java.io.File
import java.nio.file.Path
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter
import kotlin.io.path.createDirectories
import kotlin.system.exitProcess

fun main(args: Array<String>) {
    val start = File(args.firstOrNull() ?: printUsageAndExit())
    val dirForLogs = Path.of(args.getOrNull(1) ?: printUsageAndExit())
    dirForLogs.createDirectories()

    val formatter = DateTimeFormatter.ofPattern("HH.mm_dd.MM.yyyy")
    val fileName = formatter.format(LocalDateTime.now()) + ".xlsx"

    StatsProvider(Logger(dirForLogs.resolve(fileName))).use { provider ->
        provider.processDir(start).await()
    }
}

fun printUsageAndExit(): String {
    println(
        """
        file-logger <path> <path for logs>
    """.trimIndent()
    )
    exitProcess(-1)
}
